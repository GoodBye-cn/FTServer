#include "Handler.h"
#include "Worker.h"

#include <event2/event.h>

#include <stdio.h>

Handler::Handler() {
    this->worker = new Worker();
    worker->set_handler(this);
}
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

/**
 * @details bufferevent的写回调函数，当bufferevent的output evbuffer缓冲区发送完成后被调用。需要先主动调用 bufferevent_write 函数
 */
void Handler::write_cb(struct bufferevent* bev, void* ctx) {
    printf("write callback\n");
}

void Handler::event_cb(struct bufferevent* bev, short what, void* ctx) {
    printf("event callback\n");
}