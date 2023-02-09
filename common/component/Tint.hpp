#pragma once

#include "../../sfml/Color.hpp"
#include <memory>
#include <string>

namespace SFML {
/**
 * @brief Component that allows an entity to be tinted
 */
struct Tint {
    /**
     * @brief Construct a new Tint Component
     *
     * @param color_id ID of the color to apply
     */
    Tint(const std::string& color_id = "")
        : id(color_id) {};
    std::string id;
};
}
