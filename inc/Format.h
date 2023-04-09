#ifndef FORMAT_H
#define FORMAT_H

#define PATHLEN 1024

struct Request {
    int length;
    char path[PATHLEN];
};

struct Response {
    unsigned int size;
    char end[4];
};


#endif