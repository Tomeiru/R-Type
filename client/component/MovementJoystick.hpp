#pragma once

#include "../sfml/Joystick.hpp"

namespace SFML {
    /**
     * @brief Component that allows an entity to be moved with a joystick
     */
    struct MovementJoystick {
        /**
         * @brief Construct a new MovementJoystick Component
         * 
         * @param speed_value Speed of the entity
         */
        MovementJoystick(float speed_value = 1) : speed(speed_value) {}
        float speed;
    };
}