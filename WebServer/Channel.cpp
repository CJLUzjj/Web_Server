//by Zhujunjie
#include "Channel.h"
#include "EventLoop.h"
#include "Logging.h"

#include <assert.h>
#include <sstream>



Channel::Channel(EventLoop* loop, int fd)
	:loop_(loop),
	 fd_(fd),
	 events_(0),
	 revent_(0),
	 index_(-1),
	 EventHanding_(false),
	 addToLoop_(false)
{
}


Channel::~Channel()
{
	assert(!EventHanding_);//不能在执行event事件时析构，会造成程序崩溃，主要原因是eventhanding调用Close可能造成Channel析构
	assert(!addToLoop_);
	//if (loop_->isInLoopThread()) {
	//	assert(loop_->hasChannel(this));
	//}
	//std::cout << "Channel destroy" << std::endl;
	std::cout<<"free channel"<<fd_<<std::endl;
}

void Channel::updata()
{
	addToLoop_ = true;
	loop_->updataChannel(this);
}

void Channel::remove()
{
	assert(isNoneEvent());
	addToLoop_ = false;
	loop_->removeChannel(this);
}

void Channel::handleEvents()
{
	EventHanding_ = true;
	LOG << eventsToString(fd_, revent_);

	if ( (revent_ & EPOLLHUP) && !(revent_ & EPOLLIN) ){
		LOG << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
		if(CloseCallBack_)
			CloseCallBack_();
	}

	if (revent_&EPOLLERR) {
		if (ErrorCallBack_)
			ErrorCallBack_();
	}

	if (revent_&(EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
		if (ReadCallBack_)
			ReadCallBack_();
	}

	if (revent_&EPOLLOUT) {
		if (WriteCallBack_)
			WriteCallBack_();
	}
	if(ConnCallBack_)
		ConnCallBack_();
	EventHanding_ = false;
}

std::string Channel::eventsToString(int fd, int ev)
{
	std::ostringstream oss;
	oss << fd << ": ";
	if (ev & EPOLLIN)
		oss << "IN ";
	if (ev & EPOLLPRI)
		oss << "PRI ";
	if (ev & EPOLLOUT)
		oss << "OUT ";
	if (ev & EPOLLHUP)
		oss << "HUP ";
	if (ev & EPOLLRDHUP)
		oss << "RDHUP ";
	if (ev & EPOLLERR)
		oss << "ERR ";

	return oss.str();
}