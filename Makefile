CXXFLAGS:=$(CXXFLAGS) -std=c++11
TestProg: thread_pool.o TestProg.o
	$(CXX) TestProg.o thread_pool.o -lpthread -o TestProg
thread_pool.o:
	$(CXX) $(CXXFLAGS) -c thread_pool.cpp
TestProg.o:
	$(CXX) $(CXXFLAGS) -c TestProg.cpp

clean:
	rm *.o
