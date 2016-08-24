VERSION=1.0.0.1.2
CXXFLAGS:=$(CXXFLAGS) -std=c++14 -Wall -g
OBJS=thread_pool.o
thread_pool.o:
	$(CXX) $(CXXFLAGS) -c thread_pool.cpp
release:
	ar -rv libthreadpool.a $(OBJS)
install:
	cp *.h /opt/STSLib/include
	cp libthreadpool.a /opt/STSLib
clean:
	rm *.o libthreadpool.a
