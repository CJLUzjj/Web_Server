#pragma once
#include <memory>

class EventLoop;
class EventLoopThreadPool;
class Channel;

class Server
{
public:
	Server(EventLoop* baseloop, int num, int port);
	~Server();
	void start();
	void connHanding();
	EventLoop* getLoop();

private:
	EventLoop* baseloop_;
	std::string name_ = "thread_pool";
	int threadnum_;
	int port_;
	int listenfd_;
	std::unique_ptr<EventLoopThreadPool> EventLoops_;
	std::shared_ptr<Channel> AcceptChannel_;
	bool started_;
	static const int MAXFDS = 100000;
};

