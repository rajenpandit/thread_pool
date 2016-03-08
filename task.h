#ifndef __TASK_H_25022016_RAJEN__
#define __TASK_H_25022016_RAJEN__
#include <tuple>
#include <functional>
#include <iostream>
#include <memory>
//#include <type_traits>

class task_base
{
public:
	task_base()=default;
	task_base(std::unique_ptr<task_base>&& task) : _task(std::move(task)){
	}
	task_base(task_base& tb)=delete;
	void operator=(task_base& tb)=delete;
	task_base(task_base&& tb){
		_task=std::move(tb._task);
	}
	void operator=(task_base&& tb){
		_task=std::move(tb._task);
	}	
public:
	virtual void operator () (){
		(*_task)();
	};
protected:
	std::unique_ptr<task_base> _task;
};

template<typename... Ts>
class task : public task_base
{
	template <std::size_t... T>
		struct helper_index{
		};

	template <std::size_t N, std::size_t... T>
		struct helper_gen_seq : helper_gen_seq<N-1, N-1, T...>{
		};

	template <std::size_t... T>
		struct helper_gen_seq<0, T...> : helper_index<T...>{
		};


public:	
	task(std::function<void (Ts...)> func, Ts&&... args) : _function(func), _args(std::forward<Ts>(args)...){
	}
	task(const task<Ts...>& t)=delete;
	task& operator = (const task<Ts...>& t) = delete;
	task(task<Ts...>&& t){
		swap(*this, t);
	}
	task& operator = (task&& t){
		swap(*this, t);
	}
public:
	template <typename... Args, std::size_t... Is>
	void helper_func(std::tuple<Args...>& tup, helper_index<Is...>){
		_function(std::get<Is>(tup)...);
	}
	
	template <typename... Args>
	void helper_func(std::tuple<Args...>& tup){
		helper_func(tup, helper_gen_seq<sizeof...(Args)>{});
	}
public:
	void operator () (){
		helper_func(_args);
	}
private:
	friend void swap(task<Ts...>& a, task<Ts...>& b){
		std::swap(a._function, b._function);
		std::swap(a._args, b._args);	
	}
private:
	std::function<void (Ts...)> _function;
	std::tuple<Ts...> _args;
};


template <typename F, typename... Args>
task_base make_task(F&& function, Args&&... args)
{
	task_base tb(std::unique_ptr<task_base>(new task<Args...>(std::forward<F>(function), std::forward<Args>(args)...)));
	return tb;
}
#if 0

template <typename R, typename T, 
	 typename... Args>
class mf_helper{
private:
	typedef R (T::*F)(Args...);
	F f_;
public:
	typedef mf_helper<R,T,Args...> type;
	explicit mf_helper(F f) : f_(f){}
	R operator()(T &p, Args&&... args){
		return (p.*f_)(std::forward<Args>(args)...);
	}
};

template <typename F, typename T, typename... Args>
task_base make_task(F&& function,T& t , Args&&... args)
{
	typedef typename mf_helper<void,T,Args...>::type mf_type;
	task_base tb(std::unique_ptr<task_base>(new task<mf_type,T,Args...>(mf_helper<void,T,Args...>(std::forward<F>(function)),t, std::forward<Args>(args)...)));
	return tb;
}
#endif
#endif
