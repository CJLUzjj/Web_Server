#include "Operation.h"
#include "Logging.h"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


int socket_bind_listen(int port)
{
	// ���portֵ��ȡ��ȷ���䷶Χ
	if (port < 0 || port > 65535)
		return -1;

	// ����socket(IPv4 + TCP)�����ؼ���������
	int listen_fd = 0;
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;

	// ����bindʱ"Address already in use"����
	int optval = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		return -1;

	// ���÷�����IP��Port���ͼ�����������
	struct sockaddr_in server_addr;
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons((unsigned short)port);
	if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		return -1;

	// ��ʼ���������ȴ����г�ΪLISTENQ
	if (listen(listen_fd, 2048) == -1)
		return -1;
	LOG << "listen  port:" << port;

	// ��Ч����������
	if (listen_fd == -1)
	{
		close(listen_fd);
		return -1;
	}
	return listen_fd;
}

void handle_for_sigpipe()
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGPIPE, &sa, NULL))
		return;
}

int setSocketNonBlocking(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag == -1)
		return -1;

	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) == -1)
		return -1;
	return 0;
}

void httpinit(EventLoop* loop)
{
	printf("init(): pid = %d, tid = %d, loop = %p\n",
		getpid(), CurrentThread::tid(), loop);
}

ssize_t writen(int fd, void *buff, size_t n)
{
	size_t nleft = n;
	ssize_t nwritten = 0;
	ssize_t writeSum = 0;
	char *ptr = (char*)buff;
	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0)
			{
				if (errno == EINTR)
				{
					nwritten = 0;
					continue;
				}
				else if (errno == EAGAIN)
				{
					return writeSum;
				}
				else
					return -1;
			}
		}
		writeSum += nwritten;
		nleft -= nwritten;
		ptr += nwritten;
	}
	return writeSum;
}

ssize_t writen(int fd, std::string &sbuff)
{
	size_t nleft = sbuff.size();
	ssize_t nwritten = 0;
	ssize_t writeSum = 0;
	const char *ptr = sbuff.c_str();
	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0)
			{
				if (errno == EINTR)
				{
					nwritten = 0;
					continue;
				}
				else if (errno == EAGAIN)
					break;
				else
					return -1;
			}
		}
		writeSum += nwritten;
		nleft -= nwritten;
		ptr += nwritten;
	}
	if (writeSum == static_cast<int>(sbuff.size()))
		sbuff.clear();
	else
		sbuff = sbuff.substr(writeSum);
	return writeSum;
}

ssize_t readn(int fd, std::string &inBuffer, bool &zero)
{
	ssize_t nread = 0;
	ssize_t readSum = 0;
	while (true)
	{
		char buff[4096];
		if ((nread = read(fd, buff, 4096)) < 0)
		{
			if (errno == EINTR)
				continue;
			else if (errno == EAGAIN)
			{
				return readSum;
			}
			else
			{
				perror("read error");
				return -1;
			}
		}
		else if (nread == 0)
		{
			//printf("redsum = %d\n", readSum);
			zero = true;
			break;
		}
		//printf("before inBuffer.size() = %d\n", inBuffer.size());
		//printf("nread = %d\n", nread);
		readSum += nread;
		//buff += nread;
		inBuffer += std::string(buff, buff + nread);
		//printf("after inBuffer.size() = %d\n", inBuffer.size());
	}
	return readSum;
}