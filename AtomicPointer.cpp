#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int*> result{nullptr};

int main() {
    std::jthread producer{
        [] {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
            auto p = new int{69};
            result.store(p, std::memory_order_release);
        }
    };

    int counter{0};
    while (result.load(std::memory_order_acquire) == nullptr) {
        ++counter;
    }

    std::cout << "result = " << *result << std::endl;
    std::cout << "counter = " << counter << std::endl;
    delete result;

    return 0;
}
