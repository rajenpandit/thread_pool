VERSION=3.0.0.0.0
CXXFLAGS:=$(CXXFLAGS) -std=c++11 -Wall -g
OBJS=thread_pool.o
TestProg: $(OBJS) TestProg.o  release
	$(CXX) TestProg.o $(OBJS) -lpthread -o TestProg
	rm $(OBJS) TestProg.o
thread_pool.o:
	$(CXX) $(CXXFLAGS) -c thread_pool.cpp
TestProg.o:
	$(CXX) $(CXXFLAGS) -c TestProg.cpp
release:
	ar -rv libthreadpool.a $(OBJS)
install:
	cp *.h /opt/STSLib/include
	cp libthreadpool.a /opt/STSLib
clean:
	rm *.o libthreadpool.a
