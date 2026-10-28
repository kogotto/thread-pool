#pragma once

#include <Node.hpp>

template <typename T>
class EpochManager {
public:
    using Node = ::Node<T>;

    void enter(int threadId) {

    }
    void leave(int threadId) {

    }
    void retire(Node* node) {

    }
    void tryAdvance() {

    }
private:
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
    EpochManager<int>* manager_;
    int threadId_;
};
