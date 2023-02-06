#pragma once

#include <string>

namespace SFML {
/**
 * @brief Component that allows an entity to have an outline
 */
struct Outline {
    /**
     * @brief Construct a new Outline Component
     *
     * @param color_id ID of the color of the outline
     * @param thickness Thickness of the outline
     */
    Outline(const std::string& color_id = "", float thickness = 0.0)
        : id(color_id)
        , thickness(thickness) {};
    std::string id;
    float thickness;
};
}
