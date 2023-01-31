#pragma once

#include <string>

namespace SFML {
    struct Outline {
        Outline(const std::string &color_id = "", float thickness = 0.0) : id(color_id), thickness(thickness) {};
        std::string id;
        float thickness;
    };
}
