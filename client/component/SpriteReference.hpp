#pragma once

#include <string>

namespace SFML {
    struct SpriteReference {
        SpriteReference(const std::string &sprite_id = "") : id(sprite_id) {};
        std::string id;
    };
}
