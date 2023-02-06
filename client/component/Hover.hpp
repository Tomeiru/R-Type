#pragma once

namespace SFML {
/**
 * @brief Component that allows an entity to be hovered
 */
struct Hover {
    /**
     * @brief Construct a new Hover Component
     *
     * @param hovered Whether the entity is hovered or not
     */
    Hover(bool hovered = false)
        : hovered(hovered) {};
    bool hovered;
};
}
