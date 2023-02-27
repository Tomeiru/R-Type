#pragma once

#include "../../ecs/Types.hpp"
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
    DestroyEntity(ECS::Entity entity = 0, std::string spriteId = "")
        : _entity(entity)
    {
        for (int i = 0; i < 15; i++) {
            sprite_id[i] = (i < spriteId.size() ? spriteId[i] : '\0');
        }
        sprite_id[15] = '\0';
    };

    char sprite_id[16];
    ECS::Entity _entity;
};
};