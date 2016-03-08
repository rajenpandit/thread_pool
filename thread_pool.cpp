#include "thread_pool.h"


void thread_pool::start(){
	std::lock_guard<std::mutex> lk(_queue_mutex);
	if(_is_started == false)
	{
		for(int i=0; i< _pool_size; ++i){
			_threads.push_back(std::thread(&thread_pool::run,this));
		}
		_is_started = true;
	}
}
void thread_pool::stop(){
	{
		std::lock_guard<std::mutex> lk(_queue_mutex);
		_stop_threads = true;;
		_is_started = false;
	}
	_cv.notify_all();
	for(auto& t : _threads){
		t.join();
	}
}
void thread_pool::add_task(task_base&& task){
	{
		std::lock_guard<std::mutex> lk(_queue_mutex);
		_task_queue.push(std::move(task));
	}
	_cv.notify_one();
}
void thread_pool::run(){
	while(true)
	{
		task_base t;
		{
			std::unique_lock<std::mutex> lk(_queue_mutex);
			_cv.wait(lk, [this]{return (!_task_queue.empty() || _stop_threads);});	
			if(_stop_threads)
				return;
			t = std::move(_task_queue.front());
			_task_queue.pop();
		}
//		std::cout<<"Called from:"<<std::this_thread::get_id();
		t();
	}
}
