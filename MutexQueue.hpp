#pragma once

#include <mutex>
#include <queue>
#include <optional>

template <typename T>
class MutexQueue {
public:
    void push(T value) {
        std::lock_guard lock{mutex_};
        queue_.emplace(std::move(value));
    }

    std::optional<T> pop() {
        std::lock_guard lock{mutex_};
        if (queue_.empty()) {
            return std::nullopt;
        }
        auto result = queue_.front();
        queue_.pop();
        return result;
    }
private:
    std::mutex mutex_;
    std::queue<T> queue_;
};
