# Web_Server  

这是一个基于one loop per thread + thread pool模型设计的Web Server，可以实现get,head,post请求  
 
用RAII封装了互斥锁 条件变量,另外还封装了线程 生产者消费者队列 Epoll等  
 
项目中使用了大量C++11中的std::function函数和std::bind函数用于事件的回调和绑定  
 
实现了前后端分离的异步日志  
 
感谢陈硕的《Linux多线程服务端编程》给我很大的启发，我很多的设计思路都来源于这本书
 
编译方式很简单，安装MakeFile,GCC版本能支持C++11,直接在当前目录make即可  

[测试页(我的简历)](116.62.154.184 "我的简历")（建议使用Chrome浏览器打开）
 
qq:849816104  
wechat:pnzz849816104

 
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
  
  
代码总长度(含测试代码)：3772行(主要之前想写一个网络库的，后来临时改主意写了web server所以多了蛮多的冗余代码,去掉注释和缩进的话,应该在2000行左右)  
