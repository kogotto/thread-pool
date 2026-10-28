#include <EpochManager.hpp>

namespace {

std::atomic<int> freeId{0};

} // namespace

int getThreadId() {
    const thread_local auto id = freeId.fetch_add(1, std::memory_order_release);
    return id;
}
