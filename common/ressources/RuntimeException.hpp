#pragma once

#include <exception>
#include <string>

namespace RType::Map {
    class RuntimeException : public std::exception {
    public:
        RuntimeException(const std::string &where, const std::string &error_msg) : error_msg(where+std::string(": ")+error_msg) {}
        const char *what() const noexcept override {
            return (error_msg.c_str());
        }
    private:
        std::string error_msg;
    };
}