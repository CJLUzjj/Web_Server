//by Zhujunjie
#include "ThreadPool.h"
#include "CurrentThread.h"
#include <iostream>
using namespace std;

void print(string no)
{
	cout << CurrentThread::t_cache_tid << ":" << CurrentThread::t_thread_name << "  take  " << no << endl;
}

void test(int n) {
	ThreadPool thread_pool;
	thread_pool.Set_Queue_Size(n);
	thread_pool.start(5);
	for (int i = 0; i < 100; i++) {
		char buf[32];
		snprintf(buf, sizeof buf, "task %d", i);
		thread_pool.put(bind(print, string(buf)));
		cout << "put task " << i << endl;
	}
	//一定要put倒计时！！！否则主线程会直接调用stop导致running为false
	CountDownLatch L(1);
	thread_pool.put(bind(&CountDownLatch::Countdown, &L));
	L.wait();
	//一定要join线程啊！！！！否则会导致线程崩溃的！！！血的教训！！！
	thread_pool.stop();
}
int main()
{
	test(100);
}