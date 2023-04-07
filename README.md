# FTServer
FTServer 使用libevent库
## Reactor
负责监听请求（select, epoll, poll），通过分发器（Dispatch）将请求分发给Handler或者Acceptor
### Acceptor
建立链接，创建Handler对象
### Handler
处理请求：read, write

读取数据后将数据传送给Worker，然后Worker对数据进行处理
Worker处理完成后将结果传给Handler并发送给客户端
## Worker
负责业务逻辑，可以使用线程池