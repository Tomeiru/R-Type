#pragma once

#include <queue>
#include <mutex>
#include "RuntimeException.hpp"


namespace RType::Network {
    template <typename T>
    class ThreadSafeQueue {
    public:
        void push(T element) {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(element);
        }
        T pop() {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty())
                throw RuntimeException("ThreadSafeQueue::pop", "The queue is already empty");
            T value = _queue.front();
            _queue.pop();
            return value;
        }
        bool empty() {
            std::lock_guard<std::mutex> lock(_mutex);
            return (_queue.empty());
        }
    private:
        std::queue<T> _queue;
        std::mutex _mutex;
    };
}
