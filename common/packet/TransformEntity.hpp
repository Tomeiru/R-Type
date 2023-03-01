#pragma once

#include "../../ecs/Types.hpp"
#include "../../sfml/Vector2.hpp"
#include "../component/Transform.hpp"

namespace RType::Packet {
/**
 * @brief TransformEntity is a class that represents a transform packet
 */
struct TransformEntity {
    TransformEntity(ECS::Entity entity = 0, SFML::Vector2f position = { 0.0, 0.0 }, float rotation = 0.0, SFML::Vector2f scale = { 0.0, 0.0 }, SFML::Vector2f origin = { 0.0, 0.0 })
        : _entity(entity)
        , _position(position)
        , _rotation(rotation)
        , _scale(scale)
        , _origin(origin) {};

    TransformEntity(ECS::Entity entity, SFML::Transform transform)
        : _entity(entity)
        , _position(transform.position)
        , _rotation(transform.rotation)
        , _scale(transform.scale)
        , _origin(transform.origin) {};

    ECS::Entity _entity;
    SFML::Vector2f _position;
    float _rotation;
    SFML::Vector2f _scale;
    SFML::Vector2f _origin;
};
}