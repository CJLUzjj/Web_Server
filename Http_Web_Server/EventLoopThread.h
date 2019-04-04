#pragma once
#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"

class EventLoop;

class EventLoopThread
{
public:
	typedef std::function<void(EventLoop*)> ThreadInitCallBack;
	EventLoopThread(const ThreadInitCallBack& cb, std::string& name);
	~EventLoopThread();
	EventLoop* startloop();

private:
	void threadfunc();

	EventLoop* loop_;
	bool exit_;
	Thread thread_;
	Mutex mutex_;
	Condition cond_;
	ThreadInitCallBack initcallback_;
};

