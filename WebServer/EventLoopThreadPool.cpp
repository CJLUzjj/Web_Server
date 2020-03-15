#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"



EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseloop, std::string& name, int threadnum)
	:threadnum_(threadnum),
	 name_(name),
	 baseloop_(baseloop),
	 started_(false),
	 next_(0)
{
}


EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::start(const ThreadInitCallBack& cb)
{
	assert(!started_);
	baseloop_->assertInLoopThread();

	started_ = true;

	for (int i = 0; i < threadnum_; i++) {
		//char buf[name_.size() + 32];
		//snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
		std::string buf = name_ + std::to_string(i);
		EventLoopThread* thread = new EventLoopThread(cb, buf);
		threads_.push_back(std::unique_ptr<EventLoopThread>(thread));
		loops_.push_back(thread->startloop());		//Æô¶¯Ïß³Ì
	}
	if (threadnum_ == 0 && cb) {
		cb(baseloop_);
	}
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	baseloop_->assertInLoopThread();
	assert(started_);

	EventLoop* loop = baseloop_;

	if (!loops_.empty()) {
		loop = loops_[next_];
		++next_;

		if (next_ == loops_.size()) {
			next_ = 0;
		}
	}

	return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
	baseloop_->assertInLoopThread();
	assert(started_);
	if (loops_.empty()) {
		return std::vector<EventLoop*>(1, baseloop_);
	}
	return loops_;
}