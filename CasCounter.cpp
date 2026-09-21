#include <atomic>
#include <thread>
#include <vector>

#include <CasCounter.hpp>

namespace {

std::atomic<int> counter{0};

void incrementCounter() {
    int current = counter.load(std::memory_order_relaxed);
    while (!counter.compare_exchange_weak(current, current+1, std::memory_order_relaxed));
}

void threadMain(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        incrementCounter();
    }
}

}

int casCounterMain(int threadsCount, int iterations) {
    counter.store(0, std::memory_order_release);

    std::vector<std::jthread> threads;
    threads.reserve(threadsCount);
    for (int i = 0; i < threadsCount; ++i) {
        threads.emplace_back(threadMain, iterations);
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    return counter.load(std::memory_order_relaxed);
}
