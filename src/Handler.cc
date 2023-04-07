#include "Handler.h"
#include <event2/event.h>

Handler::Handler() {}
Handler::~Handler() {}

Handler::Handler(bufferevent* bev) {
    this->bev = bev;
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

/**
 * @details 读取数据到read buff中，后面在做详细处理
 */
void Handler::read_cb(struct bufferevent* bev, void* ctx) {
    Handler* handler = (Handler*)ctx;
    char* data = handler->read_buff + handler->read_bytes;
    size_t read_bytes = handler->read_bytes;
    size_t readable_size = sizeof(handler->read_buff) - read_bytes - 1;
    size_t ret = 0;
    ret = bufferevent_read(bev, data, readable_size);
    read_bytes += ret;
    handler->read_bytes = read_bytes;
}

void Handler::write_cb(struct bufferevent* bev, void* ctx) {}
void Handler::event_cb(struct bufferevent* bev, short what, void* ctx) {}