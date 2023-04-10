#ifndef ACCEPTER_H
#define ACCEPTER_H

#include <event2/listener.h>


class Acceptor {
public:
    Acceptor(/* args */);
    ~Acceptor();
    void accept_conn(struct evconnlistener* listener,
        evutil_socket_t fd, struct sockaddr* address, int socklen,
        void* ctx);
private:
    /* data */
};

#endif