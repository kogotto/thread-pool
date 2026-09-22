#pragma once

#include <atomic>

template <bool Paused = false>
class SpinLock {
public:
    void lock() {
        while (acquired_.test_and_set(std::memory_order_acquire)) {
            if constexpr (Paused) {
                __builtin_ia32_pause();
            }
        }
    }
    void unlock() {
        acquired_.clear();
    }
private:
    std::atomic_flag acquired_{};
};
