#pragma once

#include <atomic>
#include <utility>

struct ControlBlock {
    std::atomic<int64_t> counter_;
};

template <typename T>
class MySharedPtr {
public:
    friend class Testing;

    MySharedPtr() = default;

    MySharedPtr(T* data)
        : data_{data}
        , block_{new ControlBlock(1)}
    {}

    MySharedPtr(const MySharedPtr& other)
        : data_{other.data_}
        , block_{other.block_} {
        block_->counter_.fetch_add(1, std::memory_order_relaxed);
    }

    MySharedPtr(MySharedPtr&& other)
        : data_{std::exchange(other.data_, nullptr)}
        , block_{std::exchange(other.block_, nullptr)}
    {}

    ~MySharedPtr() {
        if (data_ == nullptr) {
            return;
        }

        const int64_t current =
            block_->counter_.fetch_sub(1, std::memory_order_release);
        if (current == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete block_;
            delete data_;
        }
    }

private:
    T* data_{nullptr};
    ControlBlock* block_{nullptr};
};
