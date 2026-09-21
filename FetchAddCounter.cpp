#include <atomic>
#include <thread>
#include <vector>

#include <FetchAddCounter.hpp>

namespace {

std::atomic<int> counter{0};

void incrementCounter() {
    counter.fetch_add(1, std::memory_order_relaxed);
}

void threadMain(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        incrementCounter();
    }
}

}

int fetchAddCounterMain(int threadsCount, int iterations) {
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
