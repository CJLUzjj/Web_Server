# Web_Server

这是一个基于one loop per thread + thread pool模型设计的Web Server，可以实现get,head,post请求

用RAII封装了互斥锁 条件变量,另外还封装了线程 生产者消费者队列 Epoll等

项目中使用了大量C++11中的std::function函数和std::bind函数用于事件的回调和绑定

实现了前后端分离的异步日志

后续将更新定时器和web压力测试程序的设计(有两个star就写，实在写不动了。。。)

感谢陈硕的《Linux多线程服务端编程》给我很大的启发，我很多的设计思路都来源于https://github.com/chenshuo/muduo

编译方式很简单，安装MakeFile,GCC版本能支持C++11,直接在当前目录make即可

qq:849816104
