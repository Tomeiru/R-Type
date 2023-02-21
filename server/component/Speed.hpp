#pragma once

#include <string>

namespace SFML {
/**
 * @brief Component that gives a speed to an entity
 */
struct Speed {
    /**
     * @brief Construct a new Speed Component
     *
     * @param speed Speed of the entity
     */
    Speed(float speed = 0) : speed(speed), speedValue(0) {};
    float speed;
    unsigned int speedValue;
};
}