#pragma once

#include <cstdint>
#include "../../sfml/Rect.hpp"

namespace SFML {
    /**
     * @brief Component that give a hitbox to an entity
     */
    struct Hitbox {
        /**
         * @brief Construct a new Hitbox Component
         * 
         * @param rect Rectangle that represents the hitbox
         */
        Hitbox() : rect((FloatRect){0,0,0,0}) {}
        Hitbox(FloatRect rect) : rect(rect) {};
        FloatRect rect;
    };
}
