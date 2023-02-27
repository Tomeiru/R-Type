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
    DestroyEntity(std::string id = "")
    {
        for (int i = 0; i < 15; i++) {
            sprite_id[i] = (i < id.size() ? id[i] : '\0');
        }
        sprite_id[15] = '\0';
    };

    char sprite_id[16];
};
};