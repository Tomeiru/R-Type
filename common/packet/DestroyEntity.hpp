#pragma once

#include <string>

namespace RType::Packet {
    /**
     * @brief DestroyEntity is a class that represents a destroy entity packet
     */
    struct DestroyEntity {
        /**
         * @brief Constructs a new DestroyEntity object
         * @param id The id of the entity
         */
        DestroyEntity(std::uint32_t id = 0) : id(id) {};

        std::uint32_t id;
    };
};