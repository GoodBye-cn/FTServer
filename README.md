# FTServer
FTServer 使用libevent库

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

一个Handler中包含一个bufferevent

读取数据后将数据传送给Worker，然后Worker对数据进行处理，Worker处理完成后将结果传给Handler并发送给客户端

数据：
线程池或者线程池指针
bufferevent 指针

流程：
1. 设置读、写、事件回调函数
2. 将数据从bufferevent中取出来，封装到请求的结构体中
3. 通过ThreadPool的添加任务接口，将任务传给Worker

## Worker
负责业务逻辑，可以使用线程池