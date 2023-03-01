#pragma once

#include "../../ecs/Types.hpp"
#include <string>

namespace RType::Packet {
/**
 * @brief EntityPosition is a class that represents an enemy position packet
 */
struct EntityPosition {
    /**
     * @brief Constructs a new EntityPosition packet
     *
     * @param id The server id of the entity
     * @param x The x position of the entity
     * @param y The y position of the entity
     */
    EntityPosition(ECS::Entity entity = 0, float x = 0, float y = 0)
        : _entity(entity)
        , _x(x)
        , _y(y) {};

    ECS::Entity _entity;
    float _x;
    float _y;
};
}