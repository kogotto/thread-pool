#include <ThreadPool.hpp>

ThreadPool::ThreadPool(int workersCount) {
    workers_.reserve(workersCount);
    for (int i = 0; i < workersCount; ++i) {
        workers_.emplace_back([this]{workerMain();});
    }
}

void ThreadPool::workerMain() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock{mutex};
            cv.wait(lock, [this]{return done || !queue.empty();});
            if (done && queue.empty()) {
                return;
            }
            task = std::move(queue.front());
            queue.pop();
        }
        task();
    }
}
