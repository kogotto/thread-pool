#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool(int workersCount = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <typename Func, typename...Args>
    auto submit(Func func, Args... args) {
        using ResultType = std::invoke_result_t<Func, Args...>;
        std::promise<ResultType> promise;
        auto future = promise.get_future();
        auto task = [promise = std::move(promise),
                     ...args = std::forward<Args>(args),
                     func = std::move(func)] mutable {
            promise.set_value(
                std::invoke(func, std::forward<Args>(args)...)
            );
        };
        {
            std::lock_guard lock{mutex_};
            queue_.emplace(std::move(task));
            cv_.notify_one();
        }
        return future;
    }
private:
    void workerMain();

    std::vector<std::jthread> workers_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<std::move_only_function<void()>> queue_;
    bool done_{false};
};
