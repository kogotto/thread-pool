#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

constexpr int THREADS_COUNT = 10000;

std::atomic<int> max{0};

void threadMain(int value) {
    int current = max.load(std::memory_order_relaxed);
    while (current < value &&
            !max.compare_exchange_weak(current, value, std::memory_order_relaxed));
}

int main() {
    std::vector<std::jthread> threads;
    threads.reserve(THREADS_COUNT);
    for (int i = 0; i < THREADS_COUNT; ++i) {
        threads.emplace_back(
            threadMain,
            i
        );
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    std::cout << "max = " << max.load(std::memory_order_relaxed) << std::endl;
    return 0;
}
