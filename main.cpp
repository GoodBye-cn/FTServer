#include <iostream>
#include <functional>
#include <event2/event.h>

std::function<void()> func;

int main(int, char**) {
    std::cout << "Hello, world!\n";
    return 0;
}
