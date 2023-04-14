#ifndef HANDLER_H
#define HANDLER_H

#include <event2/bufferevent.h>
#include <mutex>
#include <string.h>
#include "Threadpool.h"

#define BUFFLEN 2048

class Worker;
class Reactor;

class Handler {
public:
    Handler();
    Handler(bufferevent* bev);
    ~Handler();

    int write_data(char* data, size_t size);
    void set_send_over(bool value);
    void set_threadpool(Threadpool<Worker>* tp);
    void set_reactor(Reactor* reactor);
    void create_buff(size_t size);
private:
    static void read_cb(struct bufferevent* bev, void* ctx);
    static void write_cb(struct bufferevent* bev, void* ctx);
    static void event_cb(struct bufferevent* bev, short what, void* ctx);
private:
    Reactor* reactor;
    Threadpool<Worker>* threadpool;
    bufferevent* bev;
    Worker* worker;
    bool send_over, buff_send_over;
    char buff[BUFFLEN];
    int buff_size, write_index;
    std::mutex buff_mutex;
};


#endif