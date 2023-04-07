#include "Acceptor.h"
#include "Handler.h"

Acceptor::Acceptor() {}
Acceptor::~Acceptor() {}

void Acceptor::accept_conn(struct evconnlistener* listener,
    evutil_socket_t fd, struct sockaddr* address, int socklen,
    void* ctx) {
    Reactor* reactor = (Reactor*)ctx;
    event_base* base = evconnlistener_get_base(listener);
    bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    Handler* handler = new Handler(bev);
    reactor->add_handler(handler);
}