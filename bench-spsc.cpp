#include <benchmark/benchmark.h>

#include <spsc_test.hpp>
#include <mutex_queue_test.hpp>

namespace {

void BM_SPSCQueueTest(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = spscTest();
        benchmark::DoNotOptimize(result);
    }
}

void BM_MutexQueueTest(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = mutexQueueTest();
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_SPSCQueueTest);
BENCHMARK(BM_MutexQueueTest);
