//by Zhujunjie
#pragma once

#include <functional>
#include <memory>
#include <sys/epoll.h>
#include <string>

class Http;
class echo;
class EventLoop;

class Channel
{
public:
	typedef std::function<void()> EventCallBack;
	Channel(EventLoop* loop, int fd);
	~Channel();
	void handleEvents();
	void setReadCallBack(EventCallBack cb)
	{
		ReadCallBack_ = std::move(cb);
	}
	void setWriteCallBack(EventCallBack cb)
	{
		WriteCallBack_ = std::move(cb);
	}
	void setCloseCallBack(EventCallBack cb)
	{
		CloseCallBack_ = std::move(cb);
	}
	void setErrorCallBack_(EventCallBack cb)
	{
		ErrorCallBack_ = std::move(cb);
	}
	void setConnCallBack_(EventCallBack cb)
	{
		ConnCallBack_ = std::move(cb);
	}
	void setHolder(std::shared_ptr<Http> info)		//һ��Ҫ��channel����http���������ü������Ϊ0����http��
	{
		http_info_ = info;
	}
	int fd()
	{
		return fd_;
	}
	int events()
	{
		return events_;
	}
	int revent()
	{
		return revent_;
	}
	void setrevent(int revent)
	{
		revent_ = revent;
	}
	void enableReading()
	{
		events_ |= HReadEvent;
		updata();
	}
	void disableReading()
	{
		events_ &= ~HReadEvent;
		updata();
	}
	void enableWriting()
	{
		events_ |= HWriteEvent;
		updata();
	}
	void disableWriting()
	{
		events_ &= ~HWriteEvent;
		updata();
	}
	void disableAll()
	{
		events_ = HNoneEvent;
		updata();
	}
	bool isReading()
	{
		return events_ & HReadEvent;
	}
	bool isWriting()
	{
		return events_ & HWriteEvent;
	}
	bool isNoneEvent()
	{
		return events_ == HNoneEvent;
	}
	int index() { return index_; }
	void set_index(int idx) { index_ = idx; }
	
	EventLoop* ownerLoop() { return loop_; }

	void remove();

	std::string eventsToString(int fd, int ev);

	std::shared_ptr<Http> http_info_;

private:
	EventLoop* loop_;
	int index_;
	void updata();//����events_�¼������յ���Epoller�е�updata
	const int HReadEvent = EPOLLIN | EPOLLPRI | EPOLLET;
	const int HWriteEvent = EPOLLOUT;
	const int HNoneEvent = 0;
	int fd_;
	int events_;//��ע�¼�
	int revent_;//epoll���ص��¼�
	EventCallBack ReadCallBack_;
	EventCallBack WriteCallBack_;
	EventCallBack CloseCallBack_;
	EventCallBack ErrorCallBack_;
	EventCallBack ConnCallBack_;
	bool EventHanding_;			//�Ƿ���ִ����Ӧ�ص�
	bool addToLoop_;			//�Ƿ����¼�ѭ����
};

