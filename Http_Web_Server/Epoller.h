//by Zhujunjie
#pragma once

#include <vector>
#include <map>

#include "EventLoop.h"

class Channel;

class Epoller
{
public:
	typedef std::vector<Channel*> ChannelList;	//返回给loop使用的类型
	Epoller(EventLoop* loop);
	~Epoller();
	void poll(int timems, ChannelList* ActiveChannel);
	void updataChannel(Channel* channel);			//根据index和event来选择操作更新事件
	void removeChannel(Channel* channel);
	void update(int operation, Channel* channel);	//根据操作更新事件
	bool haschannel(Channel* channel);
private:
	void fillActiveChannel(int numEvent, ChannelList* ActiveChannels);

	const char* operationToString(int op);

	typedef std::vector<struct epoll_event> EventList;

	int epollfd_;
	EventList events_;			//epoll_wait返回的events事件

	typedef std::map<int, Channel*> ChannelMap;
	ChannelMap Channels_;		//描述符和Channel的映射

	EventLoop* ownerLoop_;

	const int kNew = -1;
	const int kAdded = 1;
	const int kDeleted = 2;
};

