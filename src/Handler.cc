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
    char buff[BUFFLEN];
    size_t read_bytes = bufferevent_read(bev, buff, sizeof(buff));
    handler->worker->write_to_buff(buff, read_bytes);
    /* 将任务添加到线程池任务队列 */

}

/**
 * @details bufferevent的写回调函数，当bufferevent的output evbuffer缓冲区发送完成后被调用。需要先主动调用 bufferevent_write 函数
 */
void Handler::write_cb(struct bufferevent* bev, void* ctx) {
    printf("write callback\n");
    /* 将任务添加到线程池任务队列 */

}

void Handler::event_cb(struct bufferevent* bev, short what, void* ctx) {
    printf("event callback\n");
}

int Handler::write_data(char* data, size_t size) {
    return bufferevent_write(bev, data, size);
}
