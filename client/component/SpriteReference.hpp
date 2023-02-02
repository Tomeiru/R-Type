#pragma once

#include <string>

namespace SFML {
    /**
     * @brief Component that allows an entity to have a sprite
     */
    struct SpriteReference {
        /**
         * @brief Construct a new SpriteReference Component
         * 
         * @param sprite_id ID of the sprite to apply
         */
        SpriteReference(const std::string &sprite_id = "") : id(sprite_id) {};
        std::string id;
    };
}
