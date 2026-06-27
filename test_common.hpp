#pragma once

#include <vector>
#include <future>
#include <thread>

constexpr int TASKS_COUNT = 100'000'000;

using Ints = std::vector<int>;
using Promise = std::promise<Ints>;
using Future = std::future<Ints>;

inline void consume(Ints& result, int task) {
    result.push_back(task);
}
