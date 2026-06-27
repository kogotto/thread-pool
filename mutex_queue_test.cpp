#include <mutex_queue_test.hpp>

#include <chrono>

#include <test_common.hpp>
#include <MutexQueue.hpp>

namespace {

MutexQueue<int> queue;

auto now() {
    return std::chrono::steady_clock::now();
}

struct Timer {
    using TimePoint = std::chrono::steady_clock::time_point;
    using Duration = std::chrono::steady_clock::duration; // nanoseconds

    Timer()
        : start_{now()}
    {}

    Duration elapsed() const {
        return now() - start_;
    }

private:
    TimePoint start_;
};

void producerMain(Promise promise) {
    Ints times;
    times.reserve(TASKS_COUNT);
    for (int i = 0; i < TASKS_COUNT; ++i) {
        const Timer timer;
        queue.push(i);
        times.emplace_back(timer.elapsed().count());
    }
    promise.set_value(std::move(times));
}

auto consumerMain(Future future) {
    Ints result;
    result.reserve(TASKS_COUNT);
    Ints times;
    times.reserve(TASKS_COUNT);

    while (true) {
        const Timer timer;
        auto&& task = queue.pop();
        times.emplace_back(timer.elapsed().count());
        if (task) {
            consume(result, *task);
            continue;
        }
        using namespace std::chrono_literals;
        if (future.wait_for(0ms) != std::future_status::ready) {
            continue;
        }
        break;
    }

    while (true) {
        const Timer timer;
        auto&& task = queue.pop();
        if (!task) {
            break;
        }
        times.emplace_back(timer.elapsed().count());
    }

    struct Result {
        Ints producerTimes;
        Ints result;
        Ints consumerTimes;
    };

    return Result{
        future.get(),
        result,
        times
    };
}

} // namespace

size_t mutexQueueTest() {
    Promise promise;
    auto future = promise.get_future();
    std::jthread producer{producerMain, std::move(promise)};
    auto [producerTimes, result, consumerTimes] =
        consumerMain(std::move(future));
    producer.join();

    return result.size() == 0
        ? 1
        : 0;
}
