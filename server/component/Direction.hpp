#pragma once

#include <string>

namespace SFML {
/**
 * @brief Component that gives a direction. Can be used to move the entity in this direction
 */
struct Direction {
    /**
     * @brief Construct a new Direction Component
     *
     * @param angle Angle aof the direction (0 to 360)
     */
    Direction(unsigned int angle = 0) : angle(angle) {};
    unsigned int angle;
};
}