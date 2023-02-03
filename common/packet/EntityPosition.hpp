#pragma once

#include <string>

namespace RType::Packet {
    /**
     * @brief EntityPosition is a class that represents an enemy position packet
     */
    struct EntityPosition {

        EntityPosition(std::uint32_t id = 0, float x = 0, float y = 0) : id(id), x(x), y(y) {};

        std::uint32_t id;
        float x;
        float y;
    };
}