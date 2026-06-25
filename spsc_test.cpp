#include <thread>
#include <vector>
#include <future>
#include <chrono>
#include <iostream>
#include <format>

#include <SPSCQueue.hpp>

constexpr int TASKS_COUNT = 100'000'000;

SPSCQueue<int, 2048> queue;

using Ints = std::vector<int>;
using Promise = std::promise<Ints>;
using Future = std::future<Ints>;

bool check(const Ints& result) {
    const auto resultSize = result.size();
    if (resultSize < 2) {
        return true;
    }
    for (size_t i = 1; i < resultSize; ++i) {
        const auto dif = result[i] - result[i-1];
        if (dif != 1) {
            return false;
        }
    }
    return true;
}

void producerMain(Promise promise) {
    Ints misses;
    misses.reserve(TASKS_COUNT);
    for (int i = 0; i < TASKS_COUNT; ++i) {
        while (!queue.push(i)) {
            misses.push_back(i);
        }
    }
    promise.set_value(std::move(misses));
}

void consume(Ints& result, int task) {
    result.push_back(task);
}

auto consumerMain(Future future) {
    using namespace std::chrono_literals;
    Ints result;
    result.reserve(TASKS_COUNT);
    size_t misses{0};

    int task;
    while (true) {
        if (queue.pop(task)) {
            consume(result, task);
            continue;
        }
        if (future.wait_for(0ms) != std::future_status::ready) {
            ++misses;
            continue;
        }
        break;
    }

    // In this point producer already finished, so we only need read queue
    // until it ends
    while (queue.pop(task)) {
        int task;
        consume(result, task);
    }

    struct Result {
        Ints producerMisses;
        Ints result;
        size_t consumerMisses;
    };
    return Result{
        future.get(),
        result,
        misses
    };
}

int main() {
    Promise promise;
    auto future = promise.get_future();
    std::jthread producer{producerMain, std::move(promise)};
    auto [producerMisses, result, consumerMisses] = consumerMain(std::move(future));
    producer.join();
    std::cout << std::format("{} producer misses {} consumer misses\n",
            producerMisses.size(), consumerMisses);
    std::cout << std::format("result correct {}\n", check(result));

    return 0;
}
