#include <vector>

#include <ThreadPool.hpp>

constexpr int TASKS_COUNT = 8;

int main() {
    ThreadPool pool;
    std::vector<std::future<bool>> tasks;
    tasks.reserve(TASKS_COUNT);
    for (int i = 0; i < TASKS_COUNT; ++i) {
        tasks.emplace_back(pool.submit(
            [] {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(2s);
                return true;
            }
        ));
    }

    for (auto&& task : tasks) {
        task.get();
    }

    return 0;
}
