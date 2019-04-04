//by Zhujunjie
#include "BlockQueue.h"
#include "CountDownLatch.h"
#include "Thread.h"

#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>

template<typename T>
class BlockQueue;

class Test
{
public:
	Test(int numThreads)
		: latch_(numThreads)
	{
		for (int i = 0; i < numThreads; ++i)
		{
			char name[32];
			snprintf(name, sizeof name, "work thread %d", i);
			threads_.emplace_back(new Thread(
				std::bind(&Test::threadFunc, this), std::string(name)));
		}
		for (auto& thr : threads_)
		{
			thr->start();
		}
	}

	void run(int times)
	{
		printf("waiting for count down latch\n");
		latch_.wait();
		printf("all threads started\n");
		for (int i = 0; i < times; ++i)
		{
			sleep(1);
			char buf[32];
			snprintf(buf, sizeof buf, "hello %d", i);
			queue_.put(buf);
			printf("tid=%d, put data = %s, size = %zd\n", CurrentThread::tid(), buf, queue_.size());
		}
	}

	void joinAll()
	{
		for (size_t i = 0; i < threads_.size(); ++i)
		{
			queue_.put("stop");
		}

		for (auto& thr : threads_)
		{
			thr->join();
		}
	}

private:

	void threadFunc()
	{
		printf("tid=%d, %s started\n",
			CurrentThread::tid(),
			CurrentThread::thread_name());

		latch_.Countdown();
		bool running = true;
		while (running)
		{
			sleep(1);
			std::string d(queue_.take());
			printf("tid=%d, get data = %s, size = %zd\n", CurrentThread::tid(), d.c_str(), queue_.size());
			running = (d != "stop");
		}

		printf("tid=%d, %s stopped\n",
			CurrentThread::tid(),
			CurrentThread::thread_name());
	}

	BlockQueue<std::string> queue_;
	CountDownLatch latch_;
	std::vector<std::unique_ptr<Thread>> threads_;
};

//void testMove()
//{
//	BlockQueue<std::unique_ptr<int>> queue;
//	queue.put(std::unique_ptr<int>(new int(42)));
//	std::unique_ptr<int> x = queue.take();
//	printf("took %d\n", *x);
//	*x = 123;
//	queue.put(std::move(x));
//	std::unique_ptr<int> y = queue.take();
//	printf("took %d\n", *y);
//}

int main()
{
	printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());
	Test t(5);
	t.run(100);
	t.joinAll();

	//testMove();
}
