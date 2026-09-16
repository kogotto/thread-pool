#include <atomic>
#include <thread>
#include <vector>

namespace {

std::atomic<int> counter{0};

void threadMain() {
    int current = counter.load(std::memory_order_relaxed);
    while (!counter.compare_exchange_weak(current, current+1, std::memory_order_relaxed));
}

}

int casCounterMain(int threadsCount) {
    counter.store(0, std::memory_order_release);

    std::vector<std::jthread> threads;
    threads.reserve(threadsCount);
    for (int i = 0; i < threadsCount; ++i) {
        threads.emplace_back(threadMain);
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    return counter.load(std::memory_order_relaxed);
}
