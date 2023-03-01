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

    /**
     * @brief Construct a new Transform Component
     *
     * @param transform Transform to copy it in a new transform component
     */
    Transform(SFML::Transform const& transform)
        : position(transform.position)
        , rotation(transform.rotation)
        , scale(transform.scale)
        , origin(transform.origin)
    {
    }

    Vector2f position;
    float rotation;
    Vector2f scale;
    Vector2f origin;
};
}
