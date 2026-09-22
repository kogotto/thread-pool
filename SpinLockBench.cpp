#include <benchmark/benchmark.h>

#include <mutex>
#include <ranges>
#include <thread>
#include <vector>

#include <SpinLock.hpp>

namespace {

constexpr int threadsCount{4};
constexpr int iterations{1'000'000};
// constexpr int iterations{1'000'000};

template <typename Lock>
int work() {
    int64_t counter{0};
    Lock lock;

    std::vector<std::jthread> threads;
    threads.reserve(threadsCount);
    for (auto i : std::views::iota(0, threadsCount)) {
        threads.emplace_back(
            [&counter, &lock] () {
                for (auto i : std::views::iota(0, iterations)) {
                    std::lock_guard guard{lock};
                    ++counter;
                }
            }
        );
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    return counter;
}

template <typename Lock>
void BM_IncrementUnderLock(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = work<Lock>();
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_IncrementUnderLock<SpinLock<true>>);
BENCHMARK(BM_IncrementUnderLock<SpinLock<false>>);
BENCHMARK(BM_IncrementUnderLock<std::mutex>);
