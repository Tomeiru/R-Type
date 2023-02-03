#pragma once

#include <queue>
#include <mutex>
#include "RuntimeException.hpp"


namespace RType::Network {
    /**
     * @brief ThreadSafeQueue is a thread-safe queue
     * @tparam T The type of the elements in the queue
     */
    template <typename T>
    class ThreadSafeQueue {
    public:
        /**
         * @brief Pushes an element in the queue
         * @param element The element to push
         */
        void push(T element) {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(element);
        }

        /**
         * @brief Pops an element from the queue
         * @return The element popped
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
         * @brief Checks if the queue is empty
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
