#pragma once

#include <memory>
#include "../sfml/Color.hpp"

namespace SFML {
    struct Tint {
        Tint(const std::string &color_id = "") : id(color_id) {};
        std::string id;
    };
}
