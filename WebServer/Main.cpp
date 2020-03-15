//by Zhujunjie
#include "Server.h"
#include "EventLoop.h"
using namespace std;

int main()
{
	if (daemon(1, 0) < 0) {
		return -1;
	}
	EventLoop loop;
	Server http_server(&loop, 3, 80);
	http_server.start();
	loop.loop();
}
