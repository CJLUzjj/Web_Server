//by Zhujunjie
#include "Server.h"
#include "EventLoop.h"
using namespace std;

int main()
{
	EventLoop loop;
	Server http_server(&loop, 3, 80);
	http_server.start();
	loop.loop();
}
