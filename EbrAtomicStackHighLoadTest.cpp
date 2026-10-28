#include <gtest/gtest.h>

#include <algorithm>
#include <atomic>
#include <future>
#include <ranges>
#include <thread>
#include <vector>
#include <iostream>

#include <EbrAtomicStack.hpp>

namespace {

std::atomic<bool> running{true};

using Stack = EbrAtomicStack<int>;

using Threads = std::vector<std::jthread>;

using Payload = std::vector<int>;
using Future = std::future<Payload>;
using Futures = std::vector<Future>;
using Promise = std::promise<Payload>;

constexpr int PUSH_THREADS_COUNT = 100;
constexpr int POP_THREADS_COUNT = 100;
constexpr int NODES_COUNT = 10'000'000;

struct PopThreads {
    Threads popThreads;
    Futures futures;
};

std::ostream& operator<<(std::ostream& stream, const Payload& payload) {
    stream << '(';
    for (const auto p : payload) {
        stream << p << ", ";
    }
    stream << ')';
    return stream;
}

std::pair<int, int> chunk(int i, int chunks_count, int amount) {
    const int chunkSize = amount / chunks_count;
    const int begin = i * chunkSize;
    const int end = std::min(begin + chunkSize, amount);
    return {begin, end};
}

void pushThreadMain(Stack& stack, int begin, int end) {
    for (auto i : std::views::iota(begin, end)) {
        stack.push(i);
    }
}

void consume(Stack& stack, Payload& payload) {
    while (auto optValue = stack.pop()) {
        payload.emplace_back(*optValue);
    }
}

void popThreadMain(Stack& stack, Promise promise) {
    Payload payload;
    payload.reserve(NODES_COUNT / POP_THREADS_COUNT * 2);
    while (running.load(std::memory_order_acquire)) {
        consume(stack, payload);
    }
    consume(stack, payload);
    promise.set_value(std::move(payload));
}

Payload getFinalResult(Futures& futures) {
    Payload result;
    result.reserve(NODES_COUNT);
    for (auto&& future : futures) {
        Payload threadResult = future.get();
        result.insert(
            result.end(),
            std::begin(threadResult),
            std::end(threadResult)
        );
    }

    std::sort(std::begin(result), std::end(result));
    return result;
}

bool check(const Payload& payload) {
    if (payload.size() != NODES_COUNT) {
        return false;
    }
    if (payload[0] != 0) {
        return false;
    }
    for (auto i : std::views::iota(0, NODES_COUNT)) {
        if (payload[i] != i) {
            return false;
        }
    }
    return true;
}

Threads launchPushThreads(Stack& stack) {
    Threads pushThreads;
    pushThreads.reserve(PUSH_THREADS_COUNT);
    for (auto i : std::views::iota(0, PUSH_THREADS_COUNT)) {
        const auto [begin, end] = chunk(i, PUSH_THREADS_COUNT, NODES_COUNT);
        pushThreads.emplace_back(
            pushThreadMain,
            std::ref(stack),
            begin,
            end
        );
    }
    return pushThreads;
}

PopThreads launchPopThreads(Stack& stack) {
    Threads popThreads;
    popThreads.reserve(POP_THREADS_COUNT);
    Futures futures;
    futures.reserve(POP_THREADS_COUNT);
    for (auto i : std::views::iota(0, POP_THREADS_COUNT)) {
        Promise promise;
        futures.emplace_back(promise.get_future());
        popThreads.emplace_back(
            popThreadMain,
            std::ref(stack),
            std::move(promise)
        );
    }
    return {std::move(popThreads), std::move(futures)};
}

void joinThreads(Threads& threads) {
    for (auto&& thread : threads) {
        thread.join();
    }
}

} // namespace

TEST(AtomicStack, Pop) {
    Stack stack;
    Threads pushThreads = launchPushThreads(stack);
    auto&& [popThreads, futures] = launchPopThreads(stack);

    joinThreads(pushThreads);
    running.store(false, std::memory_order_release);
    joinThreads(popThreads);

    ASSERT_EQ(stack.head(std::memory_order_relaxed), nullptr);
    const auto finalResult = getFinalResult(futures);
    ASSERT_TRUE(check(finalResult));
}
