#include "Acceptor.h"
#include "Handler.h"
#include "Reactor.h"

Acceptor::Acceptor() {}
Acceptor::~Acceptor() {}

void Acceptor::accept_conn(struct evconnlistener* listener,
    evutil_socket_t fd, struct sockaddr* address, int socklen,
    void* ctx) {
    Reactor* reactor = (Reactor*)ctx;
    event_base* base = evconnlistener_get_base(listener);
    bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);

    Handler* handler = new Handler(bev);
    handler->set_threadpool(reactor->get_threadpool());
    handler->set_reactor(reactor);
    reactor->add_handler(handler);
}