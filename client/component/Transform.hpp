#pragma once

#include "../sfml/Vector2.hpp"

namespace SFML {
    struct Transform {
        Transform(const Vector2f &position = {0.0, 0.0}, float rotation = 0.0, const Vector2f &scale = {1.0, 1.0}, const Vector2f &origin = {0.0, 0.0}) : position(position), rotation (rotation), scale(scale), origin(origin) {}
        Vector2f position;
        float rotation;
        Vector2f scale;
        Vector2f origin;
    };
}
