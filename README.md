# FTServer
FTServer 使用libevent库
用来传输文件

单Reactor 多线程模式
![Reactor](https://img-blog.csdnimg.cn/20200514143929138.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1dvb19ob21l,size_16,color_FFFFFF,t_70)

## Reactor
负责监听请求（select, epoll, poll），通过分发器（Dispatch）将请求分发给Handler或者Acceptor

数据：
event_base 负责监听来自客户端的请求
Acceptor
list: Handler

流程：
1. 设置监听的端口的IP地址
2. 建立event_base
3. 设置信号处理函数
2. 使用evconnlistener_new_bind监听连接，判断是监听端口还是连接的客户端口
3. 分发给Acceptor或者Handler

### Acceptor
建立连接，创建Handler对象

流程：
1. 获得socket描述符
2. 创建Handler：使用bufferevent_socket_new创建bufferevent
3. 将Handler添加到Reactor的Handler列表里面

### Handler
处理请求：read, write

读取数据后将数据传送给Worker，然后Worker对数据进行处理，Worker处理完成后将结果传给Handler并发送给客户端

一个Handler对应一个client，一个Worker，不存在线程间资源竞争关系，一个Handler中包含一个bufferevent
1. 客户请求文件
2. 服务器读取文件
3. 客户端发送文件


**read_cb函数的功能：**

1. 读取数据放到readbuff
2. 将readbuff中的数据放到Handler中的buff中
3. 读取完成后，将任务添加到线程池中，调用Worker的process函数，开始处理

可能出现的情况：多次read，多次添加任务，再process函数里面弄好，无影响。

**write_cb函数的功能：**

判断文件是否发送完毕

通知Worker将文件数据放到bufferevent中，并通知Worker继续读取文件，多线程时，可能有多个线程同时写数据，出现错误

该回调函数触发条件：
bufferevent的写回调函数，当bufferevent的output evbuffer缓冲区发送完成后被调用。需要先主动调用 bufferevent_write 函数

数据：
线程池或者线程池指针
bufferevent 指针

流程：
1. 设置读、写、事件回调函数
2. 将数据从bufferevent中取出来，封装到请求的结构体中
3. 通过ThreadPool的添加任务接口，将任务传给Worker

## Worker
负责业务逻辑，可以将任务放到线程池

任务：
1. 解析文件请求，获取文件路径，判断路径是否正确
2. 打开文件
3. 设置发送的报头信息，主动调用bufferevent_write函数，发送报头数据
4. 如果文件未读完，读取文件数据，直接放到bufferevent中，如果文件读完了，就什么也不干

是否考虑使用http协议传输数据？

自定义协议的数据类型

```
客户端请求数据类型：
size_t len 请求的路径长度
char path[PATHLEN] 文件的路径

服务端文件响应类型：
size_t len 文件的长度
文件的比特流
```

## ThreadPool
线程池，用于管理线程

有一个任务队列，用来存放Worker对象或者其指针，将任务取出后，调用其process函数

## Task
任务：Worker

Worker应设置有处理函数

Worker->process()