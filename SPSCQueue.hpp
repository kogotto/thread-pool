#pragma once

#include <cstddef>
#include <atomic>
#include <stdexcept>

struct SPSCQueueOverflow : public std::runtime_error {
    SPSCQueueOverflow(const char* what)
        : std::runtime_error{what}
    {}
};

template <typename T, size_t N>
class SPSCQueue {
    static_assert(N > 0, "Queue can't be empty");
    static_assert((N & (N - 1)) == 0, "N must be 2^k");
public:
    bool push(T value) {
        const auto head = head_.load(std::memory_order_acquire);
        const auto tail = tail_.load(std::memory_order_relaxed);
        if (tail == head + N) {
            return false;
        }
        queue_[tail % N] = std::move(value);
        tail_.store(tail + 1, std::memory_order_release);
        return true;
    }
    bool pop(T& value) {
        const auto head = head_.load(std::memory_order_relaxed);
        const auto tail = tail_.load(std::memory_order_acquire);
        if (head == tail) {
            return false;
        }
        value = std::move(queue_[head % N]);
        head_.store(head + 1, std::memory_order_release);
        return true;
    }
private:
    T queue_[N];
    std::atomic<size_t> head_{0};
    std::atomic<size_t> tail_{0};
};
