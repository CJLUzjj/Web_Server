#pragma once

#include <functional>
#include <memory>
#include <vector>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool
{
public:
	typedef std::function<void(EventLoop*)> ThreadInitCallBack;
	EventLoopThreadPool(EventLoop* baseloop, std::string& name, int threadnum);
	~EventLoopThreadPool();
	void start(const ThreadInitCallBack& cb);//一定要加const
	EventLoop* getNextLoop();
	std::vector<EventLoop*> getAllLoops();
	bool started()
	{
		return started_;
	}

	std::string& name()
	{
		return name_;
	}

private:
	EventLoop* baseloop_;
	bool started_;
	std::string name_;
	int threadnum_;
	int next_;
	std::vector<std::unique_ptr<EventLoopThread> > threads_;
	std::vector<EventLoop*> loops_;
};

