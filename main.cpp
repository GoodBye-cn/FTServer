#include <iostream>
#include <functional>
#include <event2/event.h>

#include "Threadpool.h"

std::function<void()> func;

class Test {
public:
    Test() {}
    ~Test() {}
    void process();
};

int main(int, char**) {
    std::cout << "Hello, world!\n";

    Threadpool<Test> tp(4, 100);
    return 0;
}
