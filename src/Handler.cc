#include "Handler.h"
#include "Worker.h"
#include "Threadpool.h"
#include "Reactor.h"

#include <event2/event.h>
#include <functional>
#include <stdio.h>

Handler::Handler() {}

Handler::~Handler() {
    delete worker;
    worker = nullptr;
    bufferevent_free(bev);
}

Handler::Handler(bufferevent* bev) {
    this->bev = bev;
    this->buff_size = 0;
    this->write_index = 0;
    this->send_over = true;
    this->buff_send_over = true;
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, this);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
    this->worker = new Worker();
    worker->set_handler(this);
}

/**
 * @details 读取数据到read buff中，后面在做详细处理
 */
void Handler::read_cb(struct bufferevent* bev, void* ctx) {
    printf("read callback\n");
    Handler* handler = (Handler*)ctx;
    char buff[BUFFLEN];
    size_t read_bytes = bufferevent_read(bev, buff, sizeof(buff));
    handler->worker->write_to_buff(buff, read_bytes);
    /* 将任务添加到线程池任务队列 */
    if (handler->worker->get_status() == Worker::PARSE) {
        handler->threadpool->append(handler->worker);
    }
}

/**
 * @details bufferevent的写回调函数，当bufferevent的output evbuffer缓冲区发送完成后被调用。需要先主动调用 bufferevent_write 函数
 */
void Handler::write_cb(struct bufferevent* bev, void* ctx) {
    printf("write callback\n");
    Handler* handler = (Handler*)ctx;
    int ret;
    ret = bufferevent_write(bev, handler->write_buffer + handler->send_index, handler->write_index - handler->send_index);
    if (ret == -1) {
        perror("send error");
        return;
    }
    delete handler->write_buffer;
}

void Handler::event_cb(struct bufferevent* bev, short what, void* ctx) {
    printf("event callback\n");
    /* 客户端断开连接或者发生错误，将这个Handler从Reactor中删除，析构这个类 */
    Handler* handler = (Handler*)ctx;
    /* 相当于自己析构自己，这样好吗？有没有其他方法？采用通知的形式？ */
    handler->reactor->remove_handler(handler);
}

/**
 * 有两个线程同时操作bev，worker线程和主线程
 */
int Handler::write_data(char* data, size_t size) {
    int ret = 0;
    ret = bufferevent_write(bev, data, size);
    return ret;
}

void Handler::set_send_over(bool value) {
    this->send_over = value;
}

void Handler::set_threadpool(Threadpool<Worker>* tp) {
    this->threadpool = tp;
}

void Handler::set_reactor(Reactor* reactor) {
    this->reactor = reactor;
}

void Handler::create_write_buff(size_t size) {
    this->write_buffer = new char[size];
    if (write_buffer == nullptr) {
        perror("new write buffer error");
        exit(1);
    }
    buff_size = size;
    write_index = 0;
    send_index = 0;
    send_over = true;
}

void Handler::write_to_buff(char* data, size_t size) {
    memcpy(write_buffer + send_index, data, size);
    write_index += size;
}