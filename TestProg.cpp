#include "thread_pool.h"
#include <chrono>
#include <thread>
#include <iostream>
class A{
	public:
		A()=default;
		int printData(int &x,int y)
		{
			std::cout<<"Hello:"<<y<<" " <<x<<std::endl;
			x=20;
			return x;
		}
};

void fun(int x,int& y){
	std::cout<<"Test thread:"<<x<<std::endl;
	y=10;
}
int main(){
#if 1
	thread_pool tp(3);
	std::cout<<"Created thread pool"<<std::endl;
	tp.start();
	A obj;
	for(int i=0;i<10;i++)
	{
		int x=i+1;
	//	auto tf = 
	//	tp.add_task(make_task(&A::printData,obj,x,i));
		tp.add_task(make_task(fun,i,x));
		std::cout<<"X:"<<x<<std::endl;
		tp.add_task(make_task([](){std::cout<<"lambda"<<std::endl;}));
		//std::cout<<"Task added:"<<tf.get()<<std::endl;
	}
	std::this_thread::sleep_for(std::chrono::seconds(2));
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
