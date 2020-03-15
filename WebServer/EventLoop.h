//by Zhujunjie
#pragma once

#include <vector>
#include <atomic>
#include <memory>
#include <functional>
#include "Mutex.h"
#include "CurrentThread.h"

class Channel;
class Epoller;

class EventLoop
{
public:
	EventLoop();
	~EventLoop();
	bool isInLoopThread()
	{
		return thread_id_ == CurrentThread::tid();
	}
	void assertInLoopThread();
	void updataChannel(Channel* channel);
	void removeChannel(Channel* channel);
	bool hasChannel(Channel* channel);
	void wakeup();
	bool eventHanding()
	{
		return eventHanding_;
	}

	void loop();
	void quit();

	typedef std::function<void()> Functor;
	void runInLoop(Functor cb);
	void queueInLoop(Functor cb);

	void add_time(std::shared_ptr<Channel> request, int timeout);
private:
	typedef std::vector<Channel*> ChannelList;
	std::atomic<bool> quit_;
	bool eventHanding_;
	bool looping_;
	pid_t thread_id_;

	int wake_fd_;//���ļ���������EventLoop��������ʱҪ�ر�
	std::unique_ptr<Channel> wakeupChannel_;
	void wake_handing();//wake_fd

	ChannelList ActiveChannel_;
	Channel* CurrentActiveChannel_;

	std::unique_ptr<Epoller> Epoller_;

	mutable Mutex mutex_;

	bool callingPendingFunctors_;
	std::vector<Functor> PendingFunctors;
	void Dopendingfunctors();
};


