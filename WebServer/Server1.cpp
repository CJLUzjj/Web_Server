#include "Server.h"
#include "EventLoopThreadPool.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Operation.h"
#include "Logging.h"
#include "Http.h"

#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


Server::Server(EventLoop* baseloop, int num, int port)
	:baseloop_(baseloop),
	 threadnum_(num),
	 port_(port),
	 EventLoops_(new EventLoopThreadPool(baseloop_, name_, threadnum_)),
	 started_(false),
	 listenfd_(socket_bind_listen(port_)),
	 AcceptChannel_(new Channel(baseloop_, listenfd_))
{
	handle_for_sigpipe();
	if (setSocketNonBlocking(listenfd_) < 0)
	{
		LOG << "set socket non block failed";
		abort();
	}
}


Server::~Server()
{
}

void Server::start()
{
	EventLoops_->start(httpinit);
	AcceptChannel_->enableReading();
	AcceptChannel_->setReadCallBack(std::bind(&Server::connHanding, this));
	started_ = true;
}

EventLoop* Server::getLoop()
{
	return baseloop_;
}

void Server::connHanding()
{
	//收到连接后accept。。。。
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof (sockaddr_in));
	socklen_t client_addr_len = sizeof(struct sockaddr_in);
	int accept_fd = 0;
	while ((accept_fd = accept(listenfd_, (struct sockaddr*)&client_addr, &client_addr_len)) > 0) {
		EventLoop* loop = EventLoops_->getNextLoop();
		LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);
		if (accept_fd > MAXFDS) {
			close(accept_fd);
			continue;
		}
		if (setSocketNonBlocking(accept_fd) < 0) {
			LOG << "Set non block failed!";
			abort();
		}

		//setSocketNodelay(accept_fd);
		//setSocketNoLinger(accept_fd);

		std::shared_ptr<Http> req_info(new Http(loop, accept_fd));
		req_info->getchannel()->setHolder(req_info);
		loop->runInLoop(std::bind(&Http::init, req_info));//初始化更新要在所属线程中
	}
}