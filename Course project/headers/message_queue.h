#ifndef _ARKANOID_MESSAGE_QUEUE_H_
#define _ARKANOID_MESSAGE_QUEUE_H_

#include <mutex>
#include <queue>


template<typename T>
class message_queue
{
public:
    explicit message_queue(size_t size_limit):
        _size_limit(size_limit)
    {}

    void try_push(T&& elem) {
        std::lock_guard<decltype(_mutex)> lock(_mutex);

        if (_queue.size() >= _size_limit) {
            return;
        }

        _queue.push(std::forward<T>(elem));
    }

    bool try_pop(T& out) {
        std::lock_guard<decltype(_mutex)> lock(_mutex);

        if (_queue.empty()) {
            return false;
        }

        out = _queue.front();
        _queue.pop();
        return true;
    }

private:
    std::mutex _mutex;
    std::queue<T> _queue;
    size_t _size_limit;
};

#endif
