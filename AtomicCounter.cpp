#include <atomic>
#include <thread>
#include <vector>
#include <cassert>

constexpr int THREADS_COUNT = 10;
constexpr int ITERATIONS = 100'000'000;

std::atomic<uint32_t> counter{0};

int main() {
    std::vector<std::jthread> threads;
    threads.reserve(THREADS_COUNT);

    for (int i = 0; i < THREADS_COUNT; ++i) {
        threads.emplace_back([]{
            for (int i = 0; i < ITERATIONS; ++i) {
                counter.fetch_add(1, std::memory_order_relaxed);
                // ++counter; // std::memory_order_seq_cst 10 times slower
            }
        });
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    assert(counter == THREADS_COUNT * ITERATIONS);

    return 0;
}
