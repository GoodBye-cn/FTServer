#ifndef WORKER_H
#define WORKER_H

#include <event2/bufferevent.h>

class Handler;

class Worker {
public:
    Worker(/* args */);
    ~Worker();
    void process();
    void set_handler(Handler* handler);
private:
    Handler* handler;
};

#endif