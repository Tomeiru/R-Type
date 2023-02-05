#pragma once

#include <cstdint>

namespace RType {
    namespace Client {
        struct PlayerID {
            PlayerID(std::uint16_t id = 0) : id(id) {};
            std::uint16_t id;
        };
    } // RType
} // Client
