//by Zhujunjie
#include "Epoller.h"
#include "Logging.h"
#include "Channel.h"

#include <sys/epoll.h>
#include <unistd.h>


Epoller::Epoller(EventLoop* loop)
	:ownerLoop_(loop),
	 epollfd_(epoll_create(EPOLL_CLOEXEC)),
	 events_(16)
{
	if (epollfd_ < 0)
	{
		LOG << "EPollPoller::EPollPoller";
		abort();
	}
}

Epoller::~Epoller()
{
	close(epollfd_);
}

void Epoller::poll(int timems, ChannelList* ActiveChannel)
{
	LOG << CurrentThread::thread_name() << " fd total count " << Channels_.size();
	int numEvent = epoll_wait(epollfd_, 
						events_.data(), 
						static_cast<int>(events_.size()), 
						timems);
	int saveError = errno;
	if (numEvent > 0) {
		LOG << CurrentThread::thread_name() << " "<< numEvent << " events happened";
		fillActiveChannel(numEvent, ActiveChannel);
		if (events_.size() == numEvent) {
			events_.resize(2 * events_.size());
		}
	}
	else if (numEvent == 0) {
		LOG << CurrentThread::thread_name() << " nothing hanppened";
	}
	else 
	{
		if (saveError != EINTR)
		{
			errno = saveError;
			LOG << CurrentThread::thread_name() << " EPollPoller::poll() error!";
		}
	}
}

void Epoller::fillActiveChannel(int numEvent, ChannelList* ActiveChannel)
{
	for (int i = 0; i < numEvent; i++) {
		Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
		int fd = channel->fd();
		ChannelMap::const_iterator it = Channels_.find(fd);
		assert(it != Channels_.end());
		assert(it->second == channel);
		channel->setrevent(events_[i].events);
		ActiveChannel->push_back(channel);
	}
}

void Epoller::updataChannel(Channel* channel)
{
	int index = channel->index();
	LOG << CurrentThread::thread_name() << " fd = " << channel->fd()
		<< " events = " << channel->events() << " index = " << index;
	if (index == kNew || index == kDeleted) {		//ִ�����Ӳ���
		int fd = channel->fd();
		if (index == kNew) {
			assert(Channels_.find(fd) == Channels_.end());
			Channels_[fd] = channel;
		}
		else {
			assert(Channels_.find(fd) != Channels_.end());
			assert(Channels_[fd] == channel);
		}
		channel->set_index(kAdded);
		update(EPOLL_CTL_ADD, channel);
	}
	else         //index == kAddedִ�и��Ļ�ɾ������
	{
		int fd = channel->fd();
		assert(Channels_.find(fd) != Channels_.end());
		assert(Channels_[fd] == channel);
		assert(index == kAdded);
		if (channel->isNoneEvent()) {
			update(EPOLL_CTL_DEL, channel);
			channel->set_index(kDeleted);
		}
		else {
			update(EPOLL_CTL_MOD, channel);
		}
	}
}

void Epoller::update(int operation, Channel* channel)
{
	struct epoll_event event;
	bzero(&event, sizeof event);
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	LOG << CurrentThread::thread_name() << " epoll_ctl op = " << operationToString(operation)
		<< " fd = " << fd << " event = { " << channel->eventsToString(channel->fd(),channel->events()) << " }";
	if (epoll_ctl(epollfd_, operation, fd, &event) < 0) {
		if (operation == EPOLL_CTL_DEL)
		{
			LOG << CurrentThread::thread_name() << " epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
			abort();
		}
		else
		{
			printf("%s\n", strerror(errno));
			LOG << CurrentThread::thread_name() << " epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
			abort();
		}
	}
}

void Epoller::removeChannel(Channel* channel)
{
	int fd = channel->fd();
	LOG << CurrentThread::thread_name() << " fd = " << fd;
	assert(Channels_.find(fd) != Channels_.end());
	assert(Channels_[fd] == channel);
	assert(channel->isNoneEvent());
	int index = channel->index();
	assert(index == kAdded || index == kDeleted);
	size_t n = Channels_.erase(fd);
	(void)n;
	assert(n == 1);

	if (index == kAdded)
	{
		update(EPOLL_CTL_DEL, channel);
	}
	channel->set_index(kNew);
}

const char* Epoller::operationToString(int op)
{
	switch (op)
	{
	case EPOLL_CTL_ADD:
		return "ADD";
	case EPOLL_CTL_DEL:
		return "DEL";
	case EPOLL_CTL_MOD:
		return "MOD";
	default:
		assert(false && "ERROR op");
		return "Unknown Operation";
	}
}

bool Epoller::haschannel(Channel* channel)
{
	ChannelMap::const_iterator it = Channels_.find(channel->fd());
	return it != Channels_.end() && it->second == channel;
}

void Epoller::add_timer(std::shared_ptr<Channel> request_data, int timeout)
{
	std::shared_ptr<Http> t = request_data->http_info_;
	if(t)
		timemanager_.addTime(t, timeout);
}