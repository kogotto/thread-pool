#include <atomic>
#include <iostream>
#include <thread>

int result{0};
std::atomic<bool> ready{false};

int heavyComputation() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ms);
    return 666;
}

int main() {
    std::jthread worker{
        [] {
            result = heavyComputation();
            // ready.store(true);
            ready.store(true, std::memory_order_release);
        }
    };

    int counter{0};
    // while (!ready) {
    while (!ready.load(std::memory_order_acquire)) {
        ++counter;
    }

    std::cout << "Result = " << result << std::endl;
    std::cout << "counter = " << counter << std::endl;

    return 0;
}
