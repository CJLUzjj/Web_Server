//by Zhujunjie
#pragma once
#include "Mutex.h"
#include "Condition.h"
#include <queue>

template<typename T>
class BlockQueue
{
public:
	BlockQueue();
	~BlockQueue();
	int size() const;
	void put(T& x);
	void put(T&& x);//右值引用，move 标记
	T take();
private:
	mutable Mutex mutex_;
	Condition cond_;
	std::deque<T> Queue_;
};

template<typename T>
BlockQueue<T>::BlockQueue() :
	mutex_(),
	cond_(mutex_),
	Queue_()
{
}

template<typename T>
BlockQueue<T>::~BlockQueue()
{
}

template<typename T>
int BlockQueue<T>::size() const
{
	MutexLockGuard lock(mutex_);
	return Queue_.size();
}

template<typename T>
void BlockQueue<T>::put(T& x)
{
	MutexLockGuard lock(mutex_);
	Queue_.push_back(x);
	cond_.Notify();
}

template<typename T>
void BlockQueue<T>::put(T&& x)
{
	MutexLockGuard lock(mutex_);
	Queue_.push_back(std::move(x));
	cond_.Notify();
}

template<typename T>
T BlockQueue<T>::take()
{
	MutexLockGuard lock(mutex_);
	while (Queue_.empty()) {
		cond_.Wait();
	}
	assert(!Queue_.empty());
	T front = Queue_.front();
	Queue_.pop_front();
	return front;
}
