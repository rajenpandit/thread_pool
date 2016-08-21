#include "thread_pool.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <memory>
using namespace rpt;
class A{
	public:
		A(const A&){
//			std::cout<<"Called Copy Constructor"<<std::endl;
		}
		A()
		{
//			std::cout<<"Constructor Called"<<std::endl;
		};
		~A(){
//			std::cout<<"Destructor Called"<<std::endl;
		}
		int printData(const int &x,int y,std::string s)
		{
			std::cout<<"Thread:"<<std::this_thread::get_id()<<std::endl;
			std::cout<<s<<":"<<y<<" " <<x<<std::endl;
//			std::this_thread::sleep_for(std::chrono::seconds(5));
			return y;
		}
};

void fun(std::shared_ptr<A> obj){
//	std::cout<<"Test thread:"<<obj.use_count()<<std::endl;
	//obj->printData(3,4);
}
thread_pool tp(1);
int main(){
#if 1
//	std::cout<<"Created thread pool"<<std::endl;
	tp.start();
#if 1
	A obj;
//	make_task(&A::printData,obj,2,3);	
#endif
#if 0
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
//	std::this_thread::sleep_for(std::chrono::seconds(2));
//	tp.add_task(make_task(fun,std::make_shared<A>()));
//	std::cout<<"return val:"<<x.get();
	//fun(std::make_shared<A>());
#if 1
//	for(int i=0;i<10;i++)
	{
		int i=1;
		int x=i+1;
	//	auto tf = 
		std::cout<<"Main Thread:"<<std::this_thread::get_id()<<std::endl;
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test[8s]"),8s);
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test[9s]"),9s);
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test[3s]"),3s);
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test[15s]"),15s);
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test[1s]"),1s);
		auto future = tp.add_task(make_task(&A::printData,obj,x,2,"Test[0s]"));
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test1[0s]"));
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test2[0s]"));
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test3[0s]"));
		std::this_thread::sleep_for(std::chrono::seconds(4));
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test4[0s]"));
		std::this_thread::sleep_for(std::chrono::seconds(2));
		tp.add_task(make_task(&A::printData,obj,x,++i,"Test5[0s]"));
//		std::cout<<"Result:"<<future.get()<<std::endl;
		
//		tp.add_task(make_task(fun,i,x));
//		std::cout<<"X:"<<x<<std::endl;
		//tp.add_task(make_task([](){std::cout<<"lambda"<<std::endl;}));
		//std::cout<<"Task added:"<<tf.get()<<std::endl;
	}
#endif
	std::cout<<"for loop exited"<<std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(30));
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
