#pragma once

namespace SFML {
/**
 * @brief Component that gives health to an entity
 */
struct Health {
    /**
     * @brief Construct a new Health Component
     *
     * @param healthPoints Health point of the entity
     */
    Health(unsigned int healthPoints = 1) : healthPoints(healthPoints) {};
    unsigned int healthPoints;
};
}