#include "thread_pool.h"
#include <chrono>
#include <thread>
#include <iostream>
void fun(int x){
	std::cout<<"Test thread"<<x<<std::endl;
}
int main(){
	thread_pool tp(3);
	std::cout<<"Created thread pool"<<std::endl;
	tp.start();
	for(int i=0;i<10;i++)
	{
		tp.add_task(make_task(fun,i));
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	tp.stop();
}
