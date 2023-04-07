#include "Handler.h"
#include <event2/event.h>

Handler::Handler() {}
Handler::~Handler() {}

Handler::Handler(bufferevent* bev) {
    this->bev = bev;
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void Handler::read_cb(struct bufferevent* bev, void* ctx) {}
void Handler::write_cb(struct bufferevent* bev, void* ctx) {}
void Handler::event_cb(struct bufferevent* bev, short what, void* ctx) {}