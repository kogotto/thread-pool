#include <iostream>
#include <algorithm>
#include <vector>

#include <ThreadPool.hpp>

using Ints = std::vector<int>;
constexpr int ITERATIONS = 100'000'000;
constexpr int VECTOR_SIZE = 100'000'000;
constexpr int MAX_TASKS = 1000;

auto generateRandomVector() {
    Ints result;
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        result.emplace_back(VECTOR_SIZE - i);
    }
    return result;
}

int main() {
    ThreadPool threadPool{2};
    auto fut1 = threadPool.submit(
        [] {
            int64_t sum{0};
            for (int i = 0; i < ITERATIONS; ++i) {
                if (i % 2 == 0 || i % 3 == 0) {
                    sum += i;
                }
            }
            return sum;
        }
    );

    std::vector<std::future<int>> tasks;
    for (int i = 0; i < MAX_TASKS; ++i) {
        tasks.emplace_back(threadPool.submit(
            [] (int i) {
                return i * i;
            },
            i
        ));
    }

    auto data = generateRandomVector();
    auto fut2 = threadPool.submit(
        [] (Ints& data) {
            std::ranges::sort(data);
            return true;
        },
        std::ref(data)
    );

    for (int i = 0; i < MAX_TASKS; ++i) {
        std::cout << "task " << i << " result " << tasks[i].get() << std::endl;
    }
    std::cout << "fut1 result = " << fut1.get() << std::endl;
    fut2.get();
    std::cout << "fut2 result = " << std::ranges::is_sorted(data) << std::endl;
}
