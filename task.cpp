#include "task.h"
#include <iostream>
#include <map>

class A{
	public:
		A()=default;
		void printData(int &x,int y)
		{
			std::cout<<"Hello:"<<y<<" " <<x<<std::endl;
			x=20;
		}
};
void fun(A&& a)
{
	int x=10;
	a.printData(x, 5);
}
int main()
{
	int a=10;
#if 0
	task<int&,int> task(printData,a,6);
	task();
#endif
#if 1
	A obj;
	task_base task = make_task(&A::printData,obj,a,3);
//	task_base task = make_task(fun,obj);
	task();
	std::cout<<"a:"<<a<<std::endl;
#else
#	if 0
//	std::map<int,task_base> test;
	std::unique_ptr<task_base> task1 = make_task(printData,5,6);
	(*task1)();
	std::unique_ptr<task_base> task2 = std::move(task1);
	(*task2)();
	test[0] = task1;
	test[1] = task1;
	test[0]();
	test[1]();
#	endif
#endif
}
