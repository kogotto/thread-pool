#include <atomic>
#include <thread>
#include <vector>

#include <FetchAddCounter.hpp>

namespace {

std::atomic<int> counter{0};

void incrementCounter(std::memory_order order) {
    counter.fetch_add(1, order);
}

void threadMain(int iterations,
        std::memory_order order) {
    for (int i = 0; i < iterations; ++i) {
        incrementCounter(order);
    }
}

}

int fetchAddCounterMain(int threadsCount,
                        int iterations,
                        std::memory_order order) {
    counter.store(0, std::memory_order_release);

    std::vector<std::jthread> threads;
    threads.reserve(threadsCount);
    for (int i = 0; i < threadsCount; ++i) {
        threads.emplace_back(threadMain, iterations, order);
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    return counter.load(std::memory_order_relaxed);
}
