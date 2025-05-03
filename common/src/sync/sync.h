#ifndef SYNC_H
#define SYNC_H

#include <mutex>

template<typename T>
class synced {
    std::mutex mtx;
    T value;

public:
    template<typename Callable>
    void run(Callable&& synced) {
        std::lock_guard lk{mtx};
        synced(value);
    }

    template<typename Callable>
    decltype(auto) run_and_return(Callable&& fn) {
        std::lock_guard lk{mtx};
        return std::forward<Callable>(fn)(value);
    }

    T& get() {
        std::lock_guard lk{mtx};
        return value;
    }
};

#endif // SYNC_H