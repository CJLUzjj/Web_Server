BASEOBJS = Mutex.o Condition.o CountDownLatch.o Thread.o ThreadPool.o CurrentThread.o AppendFile.o AsyncLogging.o LogFile.o Logging.o LogStream.o
NETOBJS = echo.o Http.o Operation.o Server.o EventLoopThreadPool.o EventLoopThread.o EventLoop.o Channel.o Epoller.o Timer.o
TEST_O = Main.o
TEST_CPP = Main.cpp
GPP = g++ -std=c++11 -g -c

server : $(TEST_O) $(BASEOBJS) $(NETOBJS)
	g++ -lpthread $(TEST_O) $(NETOBJS) $(BASEOBJS) -o server

$(TEST_O) : $(TEST_CPP)
	$(GPP) $(TEST_CPP)

echo.o : echo.cpp
	$(GPP) echo.cpp

Mutex.o : Mutex.cpp
	$(GPP) Mutex.cpp

Condition.o : Condition.cpp
	$(GPP) Condition.cpp

CountDownLatch.o : CountDownLatch.cpp
	$(GPP) CountDownLatch.cpp

Thread.o : Thread.cpp
	$(GPP) Thread.cpp

ThreadPool.o : ThreadPool.cpp
	$(GPP) ThreadPool.cpp

CurrentThread.o : CurrentThread.cpp
	$(GPP) CurrentThread.cpp

AppendFile.o : AppendFile.cpp
	$(GPP) AppendFile.cpp

AsyncLogging.o : AsyncLogging.cpp
	$(GPP) AsyncLogging.cpp

LogFile.o : LogFile.cpp
	$(GPP) LogFile.cpp

Logging.o : Logging.cpp
	$(GPP) Logging.cpp

LogStream.o : LogStream.cpp
	$(GPP) LogStream.cpp

Channel.o : Channel.cpp
	$(GPP) Channel.cpp

Epoller.o : Epoller.cpp
	$(GPP) Epoller.cpp

EventLoop.o : EventLoop.cpp
	$(GPP) EventLoop.cpp

EventLoopThread.o : EventLoopThread.cpp
	$(GPP) EventLoopThread.cpp

EventLoopThreadPool.o : EventLoopThreadPool.cpp
	$(GPP) EventLoopThreadPool.cpp

Operation.o : Operation.cpp
	$(GPP) Operation.cpp

Server.o : Server.cpp
	$(GPP) Server.cpp

Http.o : Http.cpp
	$(GPP) Http.cpp

Timer.o : Timer.cpp
	$(GPP) Timer.cpp

clean:
	rm -rf *.o server

