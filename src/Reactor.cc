#include "Reactor.h"
#include "Acceptor.h"


#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <signal.h>

Reactor::Reactor() {
    this->base = event_base_new();
    this->port = 8888;
    this->ip = "0.0.0.0";
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
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

    listener = evconnlistener_new_bind(base, accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (sockaddr*)&addr, sizeof(addr));

    if (listener == NULL) {
        perror("Couldn't create listener");
        return;
    }

    event_base_dispatch(base);
}

void Reactor::accept_conn_cb(struct evconnlistener* listener,
    evutil_socket_t fd, struct sockaddr* address, int socklen,
    void* ctx) {
    Reactor* reactor = (Reactor*)ctx;
    
    event_base* base = evconnlistener_get_base(listener);
    bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
}

void Reactor::sigquit_cb(evutil_socket_t, short, void*) {
    printf("SIGQUIT\n");
}