#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdint>
#include <vector>

#include <Node.hpp>

using Epoch = uint64_t;
using AtomicEpoch = std::atomic<Epoch>;
static_assert(AtomicEpoch::is_always_lock_free);

constexpr size_t MAX_THREADS_COUNT = 1000;
constexpr Epoch THREAD_NOT_ACTIVE = 0;
constexpr Epoch START_EPOCH = THREAD_NOT_ACTIVE + 1;
constexpr Epoch EPOCHS_COUNT = 3;

template <typename T>
class EpochManager {
public:
    using Node = ::Node<T>;

    ~EpochManager() {
        for (auto&& threadRetired : retired_) {
            for (auto&& epochRetired : threadRetired) {
                deleteNodes(epochRetired);
            }
        }
    }

    void enter(int threadId) {
        threadEpochs_[threadId].store(
            globalEpoch_.load(std::memory_order_acquire),
            std::memory_order_release
        );
    }

    void leave(int threadId) {
        threadEpochs_[threadId].store(
            THREAD_NOT_ACTIVE,
            std::memory_order_release
        );
    }

    void retire(int threadId, Node* node) {
        const auto epoch =
            threadEpochs_[threadId].load(std::memory_order_relaxed);
        retired_[threadId][epoch % EPOCHS_COUNT].push_back(node);
    }

    void tryAdvance(int threadId) {
        auto currentEpoch = globalEpoch_.load();
        if (!allThreadsInCurrentEpoch(currentEpoch)) {
            return;
        }

        if (!globalEpoch_.compare_exchange_strong(
                    currentEpoch, currentEpoch + 1)) {
            return;
        }

        const auto safeEpoch = (currentEpoch - 2 + EPOCHS_COUNT) % EPOCHS_COUNT;
        deleteNodes(retired_[threadId][safeEpoch]);
    }
private:
    void deleteNodes(std::vector<Node*>& nodes) {
        for (auto node : nodes) {
            delete node;
        }
        nodes.clear();
    }

    bool allThreadsInCurrentEpoch(Epoch currentEpoch) {
        auto fromAtomic =
            [] (const AtomicEpoch& epoch) {
                return epoch.load(std::memory_order_acquire);
            };
        auto inCurrentEpoch =
            [currentEpoch] (Epoch epoch) {
                return epoch == THREAD_NOT_ACTIVE || epoch == currentEpoch;
            };
        return std::ranges::all_of(
            threadEpochs_,
            inCurrentEpoch,
            fromAtomic
        );
    }

    AtomicEpoch globalEpoch_{START_EPOCH};
    std::array<AtomicEpoch, MAX_THREADS_COUNT> threadEpochs_{};
    std::array<
        std::array<std::vector<Node*>, EPOCHS_COUNT>,
        MAX_THREADS_COUNT
    > retired_{};
};

template <typename T>
class EpochGuard {
public:
    EpochGuard(EpochManager<T>& epochManager, int threadId)
        : manager_{&epochManager}
        , threadId_{threadId} {
        manager_->enter(threadId);
    }

    void leave() {
        if (manager_ != nullptr) {
            manager_->leave(threadId_);
            manager_ = nullptr;
        }
    }

    ~EpochGuard() {
        leave();
    }

private:
    EpochManager<T>* manager_;
    int threadId_;
};
