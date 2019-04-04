//by Zhujunjie
#pragma once

#include <vector>
#include <map>

#include "EventLoop.h"

class Channel;

class Epoller
{
public:
	typedef std::vector<Channel*> ChannelList;	//���ظ�loopʹ�õ�����
	Epoller(EventLoop* loop);
	~Epoller();
	void poll(int timems, ChannelList* ActiveChannel);
	void updataChannel(Channel* channel);			//����index��event��ѡ����������¼�
	void removeChannel(Channel* channel);
	void update(int operation, Channel* channel);	//���ݲ��������¼�
	bool haschannel(Channel* channel);
private:
	void fillActiveChannel(int numEvent, ChannelList* ActiveChannels);

	const char* operationToString(int op);

	typedef std::vector<struct epoll_event> EventList;

	int epollfd_;
	EventList events_;			//epoll_wait���ص�events�¼�

	typedef std::map<int, Channel*> ChannelMap;
	ChannelMap Channels_;		//��������Channel��ӳ��

	EventLoop* ownerLoop_;

	const int kNew = -1;
	const int kAdded = 1;
	const int kDeleted = 2;
};

