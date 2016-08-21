
#ifndef __THREAD_POOL__29022016_RAJEN__
#define __THREAD_POOL__29022016_RAJEN__
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <condition_variable>
#include "task.h"
namespace rpt{
/**
 * Provides interface to create a pool of threads.
 * All threads will be idle untill #rpt::task is added by using #add_task.
 * @author Rajendra Pandit (rajenpandit)
 * @bug No known bugs
 */
class thread_pool
{
public:
	thread_pool(int pool_size); 
	
	thread_pool(thread_pool &) = delete;
	void operator = (thread_pool &) = delete;
	thread_pool(thread_pool&& tp){
		swap(*this, tp);
	}
	void operator = (thread_pool&& tp){
		swap(*this, tp);
	}
public:
	void start();
	void stop();
	/*! 
	 * add task to task_que, which will be picked and invoked by a thread from thread_pool.
	 * @param task: Its a std::unique_ptr to task object which will be moved and kept in std::priority_queue
	 * @param priority: priority value will be added as waiting period (value in milliseconds).
	 */
	template<typename T>
	task_future<typename T::result_type> add_task(std::unique_ptr<T>&& task, unsigned int priority = 0){
		task_future<typename T::result_type> f = task->get_future();
		add_task(task_base(std::move(task)),std::chrono::milliseconds(priority));
		return f;
	}
	/*! 
	 * add task to task_que, which will be picked and invoked by a thread from thread_pool.
	 * @param task: Its a std::unique_ptr to task object which will be moved and kept in std::priority_queue
	 * @param priority: waiting period tells thread_pool to invoke the function once waiting period is over.
	 */
	template<typename T>
	task_future<typename T::result_type> add_task(std::unique_ptr<T>&& task,
			std::chrono::milliseconds waiting_period ){
		task_future<typename T::result_type> f = task->get_future();
		add_task(task_base(std::move(task)),waiting_period);
		return f;
	}
	bool is_started() const{
		return _is_started;
	}
private:
	void run();
	void add_task(task_base&& task,std::chrono::milliseconds waiting_period );
	friend void swap(thread_pool& tp1, thread_pool& tp2){
		std::lock_guard<std::mutex> lk1(tp1._queue_mutex);
		std::lock_guard<std::mutex> lk2(tp2._queue_mutex);
		std::swap(tp1._pool_size, tp2._pool_size);
		std::swap(tp1._stop_threads, tp2._stop_threads);
		std::swap(tp1._task_queue, tp2._task_queue);
		std::swap(tp1._threads, tp2._threads);
	}
private:
	unsigned int _pool_size;
	bool _stop_threads;
	bool _is_started;
	std::priority_queue<task_base,std::vector<task_base>,
		std::function<bool(const task_base& lhs, const task_base& rhs)>> _task_queue;
	std::mutex _queue_mutex;
	std::mutex _cv_mutex;
	std::condition_variable _cv;
	std::vector<std::thread> _threads;
};
}
#endif
