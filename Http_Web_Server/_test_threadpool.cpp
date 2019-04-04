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
	//һ��Ҫput����ʱ�������������̻߳�ֱ�ӵ���stop����runningΪfalse
	CountDownLatch L(1);
	thread_pool.put(bind(&CountDownLatch::Countdown, &L));
	L.wait();
	//һ��Ҫjoin�̰߳�������������ᵼ���̱߳����ģ�����Ѫ�Ľ�ѵ������
	thread_pool.stop();
}
int main()
{
	test(100);
}