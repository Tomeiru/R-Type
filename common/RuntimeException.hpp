#pragma once

#include <string>

namespace RType::Network {

    /**
     * @brief Class which is used to throw runtime exceptions for the network
     */
    class RuntimeException : public std::exception {
    public:
        /**
         * @brief Constructor of the exception
         * @param where Where the exception has been thrown
         * @param error_msg Error message
         */
        RuntimeException(const std::string &where, const std::string &error_msg) : error_msg(where + std::string(": ") + error_msg) {}
        /**
         * @brief Get the error message
         * @return Error message
         */
        const char *what() const noexcept override {
            return (error_msg.c_str());
        }
    private:
        std::string error_msg;
    };
}
