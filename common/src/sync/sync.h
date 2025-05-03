#ifndef SYNC_H
#define SYNC_H

#include <functional>
#include <mutex>

template<typename T>
class synced {
    std::mutex mtx;
    T value;

public:
    void run(std::function<void(T&)> synced) {
        std::lock_guard lk{mtx};
        synced(value);
    }

    T& get() {
        return value;
    }
};

#endif //SYNC_H
