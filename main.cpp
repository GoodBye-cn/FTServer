#include <iostream>
#include <functional>
#include <event2/event.h>

#include "Threadpool.h"

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

    Test* test = new Test();
    test->process();
    return 0;
}
