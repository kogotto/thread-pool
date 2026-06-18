#include <ThreadPool.hpp>

ThreadPool::ThreadPool(int workersCount) {
    workers_.reserve(workersCount);
    for (int i = 0; i < workersCount; ++i) {
        workers_.emplace_back([this]{workerMain();});
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock{mutex_};
        done_ = true;
        cv_.notify_all();
    }
    for (auto&& worker : workers_) {
        worker.join();
    }
}

void ThreadPool::workerMain() {
    while (true) {
        std::move_only_function<void()> task;
        {
            std::unique_lock lock{mutex_};
            cv_.wait(lock, [this]{return done_ || !queue_.empty();});
            if (done_ && queue_.empty()) {
                return;
            }
            task = std::move(queue_.front());
            queue_.pop();
        }
        task();
    }
}
