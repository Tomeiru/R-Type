#pragma once

#include "../../sfml/Vector2.hpp"

namespace SFML {
/**
 * @brief Component that allows an entity to be transformed (position, rotation, scale, origin)
 */
struct Transform {
    /**
     * @brief Construct a new Transform Component
     *
     * @param position Position of the entity
     * @param rotation Rotation of the entity
     * @param scale Scale of the entity
     * @param origin Origin of the entity
     */
    Transform(const Vector2f& position = { 0.0, 0.0 }, float rotation = 0.0, const Vector2f& scale = { 1.0, 1.0 }, const Vector2f& origin = { 0.0, 0.0 })
        : position(position)
        , rotation(rotation)
        , scale(scale)
        , origin(origin)
    {
    }
    Vector2f position;
    float rotation;
    Vector2f scale;
    Vector2f origin;
};
}
