#include "thread_pool.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <memory>
class A{
	public:
		A(const A&){
			std::cout<<"Called Copy Constructor"<<std::endl;
		}
		A()
		{
			std::cout<<"Constructor Called"<<std::endl;
		};
		~A(){
			std::cout<<"Destructor Called"<<std::endl;
		}
		void printData(const int &x,int y)
		{
			std::cout<<"Hello:"<<y<<" " <<x<<std::endl;
		}
};

void fun(std::shared_ptr<A> obj){
	std::cout<<"Test thread:"<<obj.use_count()<<std::endl;
	obj->printData(3,4);
}
thread_pool tp(3);
int main(){
#if 1
	std::cout<<"Created thread pool"<<std::endl;
	tp.start();
#if 0
	A obj;
	make_task(&A::printData,obj,2,3);	
#endif
#if 1
	{
		auto obj = std::make_shared<A>();
		tp.add_task(make_task([=]{
					std::cout<<"Use count inside lambda:"<<obj.use_count()<<std::endl;
					tp.add_task(make_task(std::bind(fun,obj)));
					}));	
		std::cout<<"Use count:"<<obj.use_count()<<std::endl;
//		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
#endif
	std::this_thread::sleep_for(std::chrono::seconds(2));
//	tp.add_task(make_task(fun,std::make_shared<A>()));
//	std::cout<<"return val:"<<x.get();
	//fun(std::make_shared<A>());
#if 0
	for(int i=0;i<10;i++)
	{
		int x=i+1;
	//	auto tf = 
		tp.add_task(make_task(&A::printData,obj,x,i));
//		tp.add_task(make_task(fun,i,x));
		std::cout<<"X:"<<x<<std::endl;
		//tp.add_task(make_task([](){std::cout<<"lambda"<<std::endl;}));
		//std::cout<<"Task added:"<<tf.get()<<std::endl;
	}
#endif
	std::cout<<"Returnig from main"<<std::endl;
	tp.stop();
#else
	int i = 1;
	int y = 3;
	A obj;
	auto x = make_task(&A::printData,obj,i,y);
	//auto x = make_task(fun,i,y);
	task_future<int> tf = x->get_future();
	std::cout<<"value of i:"<<tf.get()<<std::endl;
#endif
}
