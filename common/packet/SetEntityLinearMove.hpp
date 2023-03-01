#pragma once

#include "../../ecs/Types.hpp"
namespace RType::Packet {
struct SetEntityLinearMove {
    SetEntityLinearMove(ECS::Entity entity = 0, float speed = 0.0, unsigned int direction = 0, bool newComps = true)
        : _entity(entity)
        , _speed(speed)
        , _direction(direction)
        , _newComps(newComps)
    {};

    ECS::Entity _entity;
    float _speed;
    unsigned int _direction;
    bool _newComps;
};
}