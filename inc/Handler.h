#ifndef HANDLER_H
#define HANDLER_H

#include <event2/bufferevent.h>

class Handler {
public:
    Handler();
    Handler(bufferevent* bev);
    ~Handler();
    static void read_cb(struct bufferevent* bev, void* ctx);
    static void write_cb(struct bufferevent* bev, void* ctx);
    static void event_cb(struct bufferevent* bev, short what, void* ctx);

private:
    bufferevent* bev;
};


#endif