CXXFLAGS:=$(CXXFLAGS) -std=c++14 -Wall
OBJS=thread_pool.o
TestProg: $(OBJS) TestProg.o 
	$(CXX) TestProg.o $(OBJS) -lpthread -o TestProg
thread_pool.o:
	$(CXX) $(CXXFLAGS) -c thread_pool.cpp
TestProg.o:
	$(CXX) $(CXXFLAGS) -c TestProg.cpp
release:
	ar -rv libthreadpool.a $(OBJS)
clean:
	rm *.o libthreadpool.a
