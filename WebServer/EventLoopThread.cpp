#include "EventLoopThread.h"
#include "EventLoop.h"



EventLoopThread::EventLoopThread(const ThreadInitCallBack& cb, std::string& name)
	:loop_(NULL),
	 exit_(false),
	 thread_(std::bind(&EventLoopThread::threadfunc, this), name),
	 mutex_(),
	 cond_(mutex_),
	 initcallback_(cb)
{
}


EventLoopThread::~EventLoopThread()
{
	exit_ = true;
	if (loop_ != NULL) {
		loop_->quit();
		thread_.join();
	}
}

EventLoop* EventLoopThread::startloop()//返回给线程池
{
	assert(!thread_.is_start());
	thread_.start();

	EventLoop* loop = NULL;

	while (loop_ == NULL) {
		cond_.Wait();
	}

	loop = loop_;

	return loop;
}

void EventLoopThread::threadfunc()
{
	EventLoop loop;//在栈上创建loop

	if (initcallback_)
	{
		initcallback_(&loop);
	}

	{
		MutexLockGuard lock(mutex_);
		loop_ = &loop;
		cond_.Notify();
	}

	loop.loop();

	MutexLockGuard lock(mutex_);
	loop_ = NULL;
}