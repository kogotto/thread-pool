#pragma once

#include <atomic>

class Idfier {
public:
    int threadId() {
        thread_local const int id =
            freeId_.fetch_add(1, std::memory_order_release);
        return id;
    }
private:
    std::atomic<int> freeId_{0};
};
