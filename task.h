#ifndef __TASK_H_25022016_RAJEN__
#define __TASK_H_25022016_RAJEN__
#include <tuple>
#include <functional>
#include <iostream>
#include <memory>
#include <future>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
//#include <type_traits>

using namespace std::chrono_literals;
template <typename _Res> class task_future;
class task_base
{
public:
	task_base()
	{
	};
	task_base(std::unique_ptr<task_base>&& task) : _task(std::move(task)), _priority(0),_time_point(0ms){
	}
	task_base(task_base& tb)=delete;
	void operator=(task_base& tb)=delete;
	task_base(task_base&& tb){
		_task=std::move(tb._task);
		_priority = tb._priority;
		_time_point = tb._time_point;
	}
	void operator=(task_base&& tb){
		_task=std::move(tb._task);
		_priority = tb._priority;
		_time_point = tb._time_point;
	}	
	void set_priority(unsigned int priority){
		_priority = priority;
	}
	unsigned int get_priority() const{
		return _priority;
	}
	void set_waiting_period(std::chrono::milliseconds waiting_period){
		_time_point  =  std::chrono::system_clock::now() + waiting_period;

	}
	std::chrono::system_clock::time_point get_execution_time_point() const{
		return _time_point;
	}
public:
	virtual ~task_base(){
	}
	virtual void operator () (){
		(*_task)();
	};
protected:
	std::unique_ptr<task_base> _task;
/*lesser value, higher priority*/
	unsigned int _priority;
	std::chrono::system_clock::time_point _time_point;
};


template<typename R, typename... Ts>
class task : public task_base
{
public:
	using type = task<R,Ts...>;
	using result_type = R;
private:
	template <std::size_t... T>
		struct helper_index{
		};

	template <std::size_t N, std::size_t... T>
		struct helper_gen_seq : helper_gen_seq<N-1, N-1, T...>{
		};

	template <std::size_t... T>
		struct helper_gen_seq<0, T...> : helper_index<T...>{
		};
	struct call_helper{
		call_helper(task::type& p) : _p(p)
		{}
		void operator()()
		{_p.helper_func(_p._args);}
	private:
		task::type &_p;
	};
public:	
	template <typename F>
	task(F&& func, Ts&&... args) : _pt(std::forward<F>(func)), _args(std::forward<Ts>(args)...){
	}
	task(const task& t)=delete;
	task& operator = (const task& t) = delete;
	task(task&& t){
		swap(*this, t);
	}
	task& operator = (task&& t){
		swap(*this, t);
	}
	~task(){
	}
public:
	template <typename... Args, std::size_t... Is>
	void helper_func(std::tuple<Args...>& tup, helper_index<Is...>){
		_pt(std::get<Is>(tup)...);
	}
	
	template <typename... Args>
	void helper_func(std::tuple<Args...>& tup){
		helper_func(tup, helper_gen_seq<sizeof...(Args)>{});
	}
	task_future<R> get_future(){
		return task_future<R>(*this,_pt.get_future());
	}
public:
	void operator () (){
		call_helper h(*this);
		std::call_once(_flag,h);
	}
private:
	friend void swap(task& a, task& b){
		//std::swap(a._function, b._function);
		std::swap(a._args, b._args);	
		std::swap(a._pt, b._pt);
		std::swap(a._flag, b._flag);
	}
private:
	std::packaged_task<R(Ts...)> _pt;
	std::tuple<Ts...> _args;
	std::once_flag _flag;
};

template<typename F, typename... Args, typename R=typename std::result_of<F(Args...)>::type>
std::unique_ptr<task<R,Args...>>
make_task(F&& fun, Args... args){
        return std::unique_ptr<task<R,Args...>>(new task<R,Args...>(std::forward<F>(fun),std::forward<Args>(args)...));
}

#if 0
//For future reference : decltype(std::declval<F>()(std::declval<Args>()...))
template <typename F, typename... Args, typename R=decltype(std::declval<F>()(std::declval<Args>()...))>
std::unique_ptr<task<R,Args...>>
make_task(F&& fun, Args&&... args){
	//using R=;
	return std::unique_ptr<task<R,Args...>>(new task<R,Args...>(std::forward<F>(fun),std::forward<Args>(args)...));
}

template <typename F, typename T,typename... Args, typename R=decltype((std::declval<T>().*std::declval<F>())(std::declval<Args>()...))>
//template <typename F, typename T,typename... Args, typename R=int>
std::unique_ptr<task<R,T,Args...>>
make_task(F fun, T&& obj, Args&&... args)
{
	//using R=int;
	return std::unique_ptr<task<R,T,Args...>>
		(new task<R,T,Args...>(std::function<R(T,Args...)>(std::forward<F>(fun)),
				std::forward<T>(obj),std::forward<Args>(args)...));
}
#endif
#if 0
template <typename R, typename T, typename... Ts, typename T1, typename... Args>
std::unique_ptr<task<R,T1,Ts...>>
make_task(R (T::*f)(Ts...),T1&& obj, Args&&... args)
{
	using ftype = std::function<R (T1, Ts...)>;
	return std::unique_ptr<task<R,T1,Ts...>>(new task<R,T1,Ts...>(ftype{f},std::forward<T1>(obj),std::forward<Ts>(args)...));
}
/*
template <typename R,typename... Ts, typename... Args>
std::unique_ptr<task<R,Ts...>>
make_task(R (*f)(Ts...), Args&&... args)
{
	using ftype = std::function<R (Ts...)>;
	return std::unique_ptr<task<R,Ts...>>(new task<R,Ts...>(ftype{f},std::forward<Ts>(args)...));
}
*/
#elif 0
template <typename R, typename T, typename... Ts, typename T1, typename... Args>
std::unique_ptr<task<R,T1,Args...>>
make_task(R (T::*f)(Ts...),T1&& obj, Args&&... args)
{
	using ftype = std::function<R (T1, Ts...)>;
	return std::unique_ptr<task<R,T1,Args...>>(new task<R,T1,Args...>(ftype{f},std::forward<T1>(obj),std::forward<Args>(args)...));
}
template <typename R,typename... Ts, typename... Args>
task_base make_task(R (*f)(Ts...), Args&&... args)
{
	using ftype = std::function<R (Ts...)>;
	task_base tb(std::unique_ptr<task_base>(new task<R,Args...>(ftype{f},std::forward<Args>(args)...)));
	return tb;
}
#endif
template <typename _Res>
class task_future
{
public:
	task_future(task_base& task, std::future<_Res>&& f): _task(task), _future(std::move(f)){
	}
	_Res get(){
		_task();
		return _future.get();
	}
	std::shared_future<_Res> share(){
		return _future.share();
	}
	bool valid() const{
		return _future.valid();
	}
	void wait() const{
		_future.wait();
	}
	template< class Rep, class Period >
		std::future_status wait_for( const std::chrono::duration<Rep,Period>& timeout_duration ) const{
		return _future.wait_for(timeout_duration);
	}
	template< class Clock, class Duration >
		std::future_status wait_until( const std::chrono::time_point<Clock,Duration>& timeout_time ) const{
		return _future.wait_until(timeout_time);
	}
private:
	task_base& _task;
	std::future<_Res> _future;
};

#endif
