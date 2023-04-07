#ifndef REACTOR_H
#define REACTOR_H

#include <event.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <event2/listener.h>


#include <list>

#include "Handler.h"

using namespace std;

class Acceptor;

class Reactor {
public:
    Reactor();
    ~Reactor();
    void start();

    static void accept_conn_cb(struct evconnlistener* listener,
        evutil_socket_t fd, struct sockaddr* address, int socklen,
        void* ctx);
    static void sigquit_cb(evutil_socket_t sig, short what, void* arg);
private:
    event_base* base;
    event* sigquit_event;
    evconnlistener* listener;
    sockaddr_in addr;
    short port;
    const char* ip;
    Acceptor* acceptor;
    list<Handler> handlers;
};


#endif