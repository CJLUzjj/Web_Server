#ifdef _MAIN_
#include "EventLoopThreadPool.h"
#include "EventLoop.h"

void init(EventLoop* p)
{
	printf("init(): pid = %d, tid = %d, loop = %p\n",
		getpid(), CurrentThread::tid(), p);
}

int main()
{
	std::string name = "base";
	EventLoop baseloop;
	EventLoopThreadPool pool(&baseloop, name, 3);
	pool.start(init);
	baseloop.loop();
}

#endif // !_MAIN_