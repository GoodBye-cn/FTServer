#include <iostream>
#include <functional>
#include <event2/event.h>

#include "Threadpool.h"
#include "Reactor.h"

std::function<void()> func;

class Test {
public:
    Test() {}
    ~Test() {}
    void process() {
        delete this;
    }
};

int main(int, char**) {
    std::cout << "Hello, world!\n";

    Threadpool<Worker>* tp = new Threadpool<Worker>(4, 100);
    Reactor reactor;
    reactor.set_threadpool(tp);
    reactor.start();
    return 0;
}
