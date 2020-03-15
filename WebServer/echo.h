#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include <memory>
using namespace std;

class echo:public enable_shared_from_this<echo> 
{
private:
    EventLoop* loop_;
    int fd_;
    Channel* channel_;
    bool end_;
public:
    echo(EventLoop* loop, int fd);
    ~echo();
    void init();

    void handleread();
    void handlewrite();
    void handleclose();
    void handleconn();
    Channel* getchannel(){return channel_;}
};
