#pragma once

#include "../../ecs/Types.hpp"
namespace RType::Packet {
/**
 * @brief SetEntityLinearMove is a class that represents a packet to apply a linear move to an entity
 */
struct SetEntityLinearMove {
    /**
     * @brief Construct a new SetEntityLinearMove object
     *
     * @param entity Entity to apply the linear move to
     * @param speed Speed of the move
     * @param direction Direction of the move
     * @param newComps State of the entity (false if it already has a Direction and Speed component, otherwise true)
     */
    SetEntityLinearMove(ECS::Entity entity = 0, float speed = 0.0, unsigned int direction = 0, bool newComps = true)
        : _entity(entity)
        , _speed(speed)
        , _direction(direction)
        , _newComps(newComps) {};

    ECS::Entity _entity;
    float _speed;
    unsigned int _direction;
    bool _newComps;
};
}