#include <iostream>

#include <defs.hpp>

int main() {
    for (Int i = 0; i < TASKS_COUNT; ++i) {
        std::cout << "Task " << i << " result = " << work(i) << std::endl;
    }

    return 0;
}
