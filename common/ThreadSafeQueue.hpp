#pragma once

#include <queue>
#include <mutex>
#include "RuntimeException.hpp"


namespace RType::Network {
    /**
     * @brief Thread safe queue
     * @tparam T Type of the queue
     */
    template <typename T>
    class ThreadSafeQueue {
    public:
        /**
         * @brief Push an element in the queue
         * @param element Element to push
         */
        void push(T element) {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(element);
        }
        /**
         * @brief Pop an element from the queue
         * @return Element popped
         */
        T pop() {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty())
                throw RuntimeException("ThreadSafeQueue::pop", "The queue is already empty");
            T value = _queue.front();
            _queue.pop();
            return value;
        }
        /**
         * @brief Check if the queue is empty
         * @return True if the queue is empty, false otherwise
         */
        bool empty() {
            std::lock_guard<std::mutex> lock(_mutex);
            return (_queue.empty());
        }
    private:
        std::queue<T> _queue;
        std::mutex _mutex;
    };
}
