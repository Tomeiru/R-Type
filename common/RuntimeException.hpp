#pragma once

#include <string>

namespace RType::Network {
    /**
     * @brief RuntimeException is a class that represents an exception for the network
     */
    class RuntimeException : public std::exception {
    public:
        /**
         * @brief Constructs a new RuntimeException object
         * @param where The place where the exception was thrown
         * @param error_msg The error message
         */
        RuntimeException(const std::string &where, const std::string &error_msg) : error_msg(where + std::string(": ") + error_msg) {}

        /**
         * @brief Get the error message
         * @return The error message
         */
        const char *what() const noexcept override {
            return (error_msg.c_str());
        }
    private:
        std::string error_msg;
    };
}
