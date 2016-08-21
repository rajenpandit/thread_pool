#ifndef __THREAD_POOL__29022016_RAJEN__
#define __THREAD_POOL__29022016_RAJEN__
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <condition_variable>
#include "task.h"

class thread_pool
{
public:
	thread_pool(int pool_size) : _pool_size(pool_size),_task_queue([](const task_base& lhs, const task_base& rhs)->bool{
#if 0
				if(lhs.get_priority() && rhs.get_priority())
					return (lhs.get_priority() > rhs.get_priority());
				else if(lhs.get_priority())
					return true;
				else if(rhs.get_priority())
					return false;
				else
#endif
					return (lhs.get_execution_time_point() > rhs.get_execution_time_point());})
	{
		if(_pool_size==0)
			_pool_size=1;
		_stop_threads=false;
	}	
public:
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
	template<typename T>
	task_future<typename T::result_type> add_task(std::unique_ptr<T>&& task, unsigned int priority = 0){
//		task->set_waiting_period(std::chrono::milliseconds(priority));
		task_future<typename T::result_type> f = task->get_future();
		add_task(task_base(std::move(task)),std::chrono::milliseconds(priority));
		return f;
	}
	template<typename T>
	task_future<typename T::result_type> add_task(std::unique_ptr<T>&& task,
			std::chrono::milliseconds waiting_period ){
		//task->set_waiting_period(waiting_period);
		task_future<typename T::result_type> f = task->get_future();
		add_task(task_base(std::move(task)),waiting_period);
		return f;
	}
	void run();
	bool is_started() const{
		return _is_started;
	}
private:
	void add_task(task_base&& task,std::chrono::milliseconds waiting_period );
	friend void swap(thread_pool& tp1, thread_pool& tp2){
		std::lock_guard<std::mutex> lk1(tp1._queue_mutex);
		std::lock_guard<std::mutex> lk2(tp2._queue_mutex);

		std::swap(tp1._pool_size, tp2._pool_size);
		std::swap(tp1._stop_threads, tp2._stop_threads);
		std::swap(tp1._task_queue, tp2._task_queue);
		//std::swap(std::move(tp1._queue_mutex), std::move(tp2._queue_mutex));
		//std::swap(tp1._cv, tp2._cv);
		std::swap(tp1._threads, tp2._threads);
	}
private:
	unsigned int _pool_size;
	bool _stop_threads;
	bool _is_started;
	//std::queue<task_base> _task_queue;

	std::priority_queue<task_base,std::vector<task_base>,
		std::function<bool(const task_base& lhs, const task_base& rhs)>> _task_queue;
	std::mutex _queue_mutex;
	std::mutex _cv_mutex;
	std::condition_variable _cv;
	std::vector<std::thread> _threads;
};
#endif
