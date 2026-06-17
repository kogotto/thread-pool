#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool(int workersCount);
    ~ThreadPool();
private:
    void workerMain();
    std::vector<std::jthread> workers_;
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<std::function<void()>> queue;
    bool done{false};
};
