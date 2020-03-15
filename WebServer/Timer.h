#pragma once
#include "Http.h"
#include <deque>
#include <memory>
#include <queue>

class TimeNode
{
public:
    TimeNode(std::shared_ptr<Http> requestData, int timeout);
    ~TimeNode();
    void update(int timeout);
    bool isValid();
    void clear();
    void setDeleted(){deleted_ = true;}
    bool isDeleted(){return deleted_;}
    size_t getExpTime(){return expiredTime_;}
private: 
    bool deleted_;
    size_t expiredTime_;
    std::shared_ptr<Http> http_;
};

struct TimeCmp
{
    bool operator()(std::shared_ptr<TimeNode> &a, std::shared_ptr<TimeNode> &b)
    {
        return a->getExpTime() > b->getExpTime();
    }
};

class TimeManager
{
public:
    TimeManager();
    ~TimeManager();
    void addTime(std::shared_ptr<Http> http, int timeout);
    void handleExpiredEvent();

private:
    typedef std::shared_ptr<TimeNode> node;
    std::priority_queue<node, std::deque<node>, TimeCmp> timenodequeue;
};