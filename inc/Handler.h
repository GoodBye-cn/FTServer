#ifndef HANDLER_H
#define HANDLER_H

#include <event2/bufferevent.h>
#include <string.h>

#define BUFFLEN 2048

class Worker;

class Handler {
public:
    Handler();
    Handler(bufferevent* bev);
    ~Handler();
    static void read_cb(struct bufferevent* bev, void* ctx);
    static void write_cb(struct bufferevent* bev, void* ctx);
    static void event_cb(struct bufferevent* bev, short what, void* ctx);
    int write_data(char* data, size_t size);
    int write_to_buff(char* data, size_t size);
private:
    bufferevent* bev;
    size_t read_bytes, write_bytes;
    char read_buff[BUFFLEN], write_buff[BUFFLEN];
    Worker* worker;
};


#endif