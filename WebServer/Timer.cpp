#include "Timer.h"
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <queue>

TimeNode::TimeNode(std::shared_ptr<Http> requestData, int timeout)
    :deleted_(false),
    http_(requestData)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    expiredTime_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000) + timeout);
}

TimeNode::~TimeNode()
{
    if(http_)
        http_->handleClose();
    std::cout<<"free timenode "<<this<<std::endl;
}

void TimeNode::update(int timeout)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    expiredTime_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000) + timeout);
}

bool TimeNode::isValid()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    size_t temp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
    if(temp < expiredTime_){
        return true;
    }else{
        this->setDeleted();
        return false;
    }
}

void TimeNode::clear()
{
    http_.reset();
    this->setDeleted();
}

TimeManager::TimeManager(){}

TimeManager::~TimeManager(){}

void TimeManager::addTime(std::shared_ptr<Http> http, int timeout)
{
    node new_node(new TimeNode(http, timeout));
    timenodequeue.push(new_node);
    http->time_ = new_node;
    std::cout<<"new time "<<new_node<<std::endl;
}

void TimeManager::handleExpiredEvent()
{
    while(!timenodequeue.empty()){
        node temp_node = timenodequeue.top();
        if(temp_node->isDeleted()){
            std::cout<<"free by delete"<<std::endl;
            timenodequeue.pop();
        }
        else if(temp_node->isValid() == false){
            std::cout<<"free by expired"<<std::endl;
            timenodequeue.pop();
        }
        else
            break;
    }
}