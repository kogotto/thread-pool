#include <atomic>
#include <thread>

#include <ThreeTransitiveThreads.hpp>

namespace {

std::atomic<bool> aReady{false};
int aResult{0};

void threadAMain(int result) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100us);
    aResult = result;
    aReady.store(true, std::memory_order_release);
}

std::atomic<bool> bReady{false};
int bResult{0};

void threadBMain(int result) {
    while (!aReady.load(std::memory_order_acquire)) {}
    bResult = aResult + result;
    bReady.store(true, std::memory_order_release);
}

} // namespace

Result threeTransitiveThreads(int resultA, int resultB) {
    aReady.store(false, std::memory_order_relaxed);

    std::jthread aThread{threadAMain, resultA};
    std::jthread bThread{threadBMain, resultB};

    while (!bReady.load(std::memory_order_acquire)) {}
    return {aResult, bResult};
}
