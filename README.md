# Web_Server  

## 简介
1. 这是一个基于one loop per thread + thread pool模型设计的Web Server，可以实现get,head,post请求  
 
2. 用RAII封装了互斥锁 条件变量,另外还封装了线程 生产者消费者队列 Epoll等  
 
3. 项目中使用了大量C++11中的std::function函数和std::bind函数用于事件的回调和绑定  
 
4. 实现了前后端分离的异步日志  
 
感谢陈硕的《Linux多线程服务端编程》给我很大的启发，我很多的设计思路都来源于这本书

## Envoirment

* OS: CentOS 7.6
* Complier: g++ 4.8

## Build

```
make
```

## Run

```
./server
```

测试页（我的简历）：http://zhujjqaq.xyz （请使用Chrome浏览器打开）

 
项目结构如下:  
  
  
```
AppendFile.{h,cpp}          封装日志的写操作

AsyncLogging.{h,cpp}        异步日志的实现

BlockQueue.h                生产者消费者队列(无界)

Channel.{h,cpp}             对每个Socket连接的事件分发

Condition.{h,cpp}           对条件变量的封装

CountDownLatch.{h,cpp}      "倒计时门闩"同步

CurrentThread.{h,cpp}       线程局部数据

Epoller.{h,cpp}             epoll的封装

EventLoop.{h,cpp}           事件分发器

EventLoopThread.{h,cpp}     新建一个专门用于EventLoop的线程

EventLoopThreadPool.{h,cpp} one loop per thread + thread pool模型

Http.{h,cpp}                对http请求的业务逻辑的操作封装

LogFile.{h,cpp}             对AppendFile的进一步封装

Logging.{h,cpp}             日志系统的对外接口

LogStream.{h,cpp}           对日志流操作运算符的封装

Mutex.{h,cpp}               用RAII手法对互斥锁的封装

Operation.{h,cpp}           对一些socket操作的封装

Server.{h,cpp}              TCP服务端

Thread.{h,cpp}              线程封装

ThreadPool.{h,cpp}          线程池封装
```
