#include <iostream>
#include <vector>

#include <defs.hpp>
#include <ThreadPool.hpp>

int main() {
    ThreadPool pool;
    std::vector<std::future<Int>> tasks;
    tasks.reserve(TASKS_COUNT);

    for (Int i = 0; i < TASKS_COUNT; ++i) {
        tasks.emplace_back(pool.submit(
            work,
            i
        ));
    }

    for (Int i = 0; i < TASKS_COUNT; ++i) {
        std::cout << " Task " << i <<
            " result = " << tasks[i].get() << std::endl;
    }

    return 0;
}
