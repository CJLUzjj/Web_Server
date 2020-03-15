#include "echo.h"
#include <string.h>

echo::echo(EventLoop* loop, int fd)
    :loop_(loop),
    fd_(fd),
    channel_(new Channel(loop_, fd_)),
    end_(false)
{
}

echo::~echo()
{
}

void echo::init()
{
    channel_->enableReading();
    channel_->setReadCallBack([this](){
        char buf[4096];
        memset(buf, 0, 4096);
        int size = 0;
        while(1){
            size = read(fd_, buf, sizeof(buf));
            if(size < 0){
                if(errno == EINTR)
                    continue;
                if(errno == EAGAIN)
                    break;
            }
            if(size == 0){
                loop_->queueInLoop([this](){
                    shared_ptr<echo> guard(shared_from_this());
                    channel_->disableAll();
                    channel_->remove();
                });
            }
            write(fd_, buf, size);
        }
    });
    channel_->setWriteCallBack([this](){

    });
}