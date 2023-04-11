#include "Reactor.h"
#include "Acceptor.h"
#include "Handler.h"

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/thread.h>
#include <signal.h>

Reactor::Reactor() {
    this->port = 8888;
    this->ip = "0.0.0.0";
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    evthread_use_pthreads();
    this->base = event_base_new();
    acceptor = new Acceptor();
}

Reactor::~Reactor() {
    event_free(sigquit_event);
    event_base_free(base);
    evconnlistener_free(listener);
    delete acceptor;
}

void Reactor::start() {
    /* process signal quit */
    sigquit_event = event_new(base, SIGQUIT, EV_SIGNAL | EV_PERSIST, sigquit_cb, NULL);
    event_add(sigquit_event, NULL);

    listener = evconnlistener_new_bind(base, accept_conn_cb, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (sockaddr*)&addr, sizeof(addr));

    if (listener == NULL) {
        perror("Couldn't create listener");
        return;
    }

    event_base_dispatch(base);
}

void Reactor::add_handler(Handler* handler) {
    handlers.push_back(handler);
}

/**
 * @brief 接收新的客户端连接
 * @details 通过Reactor指针，将参数传递给Acceptor的连接处理函数中
 */
void Reactor::accept_conn_cb(struct evconnlistener* listener,
    evutil_socket_t fd, struct sockaddr* address, int socklen,
    void* ctx) {
    Reactor* reactor = (Reactor*)ctx;
    reactor->acceptor->accept_conn(listener, fd, address, socklen, ctx);
}

void Reactor::sigquit_cb(evutil_socket_t sig, short what, void* ctx) {
    printf("SIGQUIT\n");
}

void Reactor::remove_handler(Handler* handler) {
    handlers.remove(handler);
    delete handler;
}

void Reactor::set_threadpool(Threadpool<Worker>* threadpool) {
    this->threadpool = threadpool;
}

Threadpool<Worker>* Reactor::get_threadpool() {
    return threadpool;
}
