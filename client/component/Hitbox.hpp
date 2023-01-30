#pragma once

#include <cstdint>
#include "../sfml/Rect.hpp"

namespace SFML {
    struct Hitbox {
        Hitbox() : rect((FloatRect){0,0,0,0}) {}
        Hitbox(FloatRect rect) : rect(rect) {};
        FloatRect rect;
    };
}
