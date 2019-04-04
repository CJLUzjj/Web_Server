//by Zhujunjie
#include "EventLoop.h"
#include "Epoller.h"
#include "Channel.h"
#include "Logging.h"

#include <sys/eventfd.h>
#include <unistd.h>



EventLoop::EventLoop()
	:looping_(false),
	 quit_(false),
	 eventHanding_(false),
	 thread_id_(CurrentThread::tid()),
	 Epoller_(new Epoller(this)),
	 CurrentActiveChannel_(NULL),
	 callingPendingFunctors_(false)
{
	int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0) {
		LOG<< "Failed in eventfd";
		abort();
	}
	wake_fd_ = evtfd;
	//create wakeupfd

	wakeupChannel_.reset(new Channel(this, wake_fd_));

	LOG << "Eventloop created in thread" << thread_id_;

	wakeupChannel_->setReadCallBack(std::bind(&EventLoop::wake_handing, this));
	wakeupChannel_->enableReading();
}


EventLoop::~EventLoop()
{
	LOG << "Eventloop of thread " << thread_id_ << " destructs in thread " << CurrentThread::tid();
	wakeupChannel_->disableAll();
	wakeupChannel_->remove();
	close(wake_fd_);
}

void EventLoop::assertInLoopThread()
{
	if (!isInLoopThread()) {
		LOG << "EventLoop::abortNotInLoopThread - EventLoop was created in threadId_ = " << thread_id_
			<< ", current thread id = " << CurrentThread::tid();
		abort();
	}
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	quit_ = false;
	LOG << "EventLoop start looping";

	while (!quit_)
	{
		ActiveChannel_.clear();
		Epoller_->poll(10000, &ActiveChannel_);
		
		for (Channel* channel : ActiveChannel_) {
			LOG << "{" << channel->eventsToString(channel->fd(), channel->revent()) << "}";
		}
		
		eventHanding_ = true;

		for (Channel* channel : ActiveChannel_) {
			CurrentActiveChannel_ = channel;
			CurrentActiveChannel_->handleEvents();
		}

		CurrentActiveChannel_ = NULL;
		eventHanding_ = false;
		Dopendingfunctors();
	}
	LOG << "EventLoop stop looping";
	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
	if (!isInLoopThread()) {
		wakeup();
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = write(wake_fd_, &one, sizeof one);
	if (n != sizeof one) {
		LOG << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventLoop::wake_handing()
{
	uint64_t one = 1;
	ssize_t n = read(wake_fd_, &one, sizeof one);
	if (n != sizeof one) {
		LOG << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
	}
}

void EventLoop::runInLoop(Functor cb)
{
	if (isInLoopThread()) {
		cb();
	}
	else {
		queueInLoop(std::move(cb));
	}
}

void EventLoop::queueInLoop(Functor cb)
{
	{
		MutexLockGuard lock(mutex_);
		PendingFunctors.push_back(std::move(cb));
	}

	if (!isInLoopThread() || callingPendingFunctors_) {
		wakeup();
	}
}

void EventLoop::Dopendingfunctors()
{
	std::vector<Functor> functors;		//ËõÐ¡ÁÙ½çÇø
	callingPendingFunctors_ = true;

	{
		MutexLockGuard lock(mutex_);
		functors.swap(PendingFunctors);
	}

	for (Functor& functor : functors) {
		functor();
	}

	callingPendingFunctors_ = false;
}

void EventLoop::updataChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	Epoller_->updataChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	if (eventHanding_) {
		assert(CurrentActiveChannel_ == channel || 
			std::find(ActiveChannel_.begin(), ActiveChannel_.end(), channel) == ActiveChannel_.end());
	}
	Epoller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	return Epoller_->haschannel(channel);
}