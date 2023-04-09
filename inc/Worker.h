#ifndef WORKER_H
#define WORKER_H

#include <event2/bufferevent.h>

#ifndef BUFFLEN
#define BUFFLEN 2048
#endif

class Handler;

class Worker {
public:
    Worker(/* args */);
    ~Worker();
    void process();
    void set_handler(Handler* handler);
    void write_to_buff(char* data, size_t size);
private:
    Handler* handler;
    char buff[BUFFLEN]; /* 存放数据的buff */
    int buff_size;      /* 当前buff中的数据大小 */
};

#endif