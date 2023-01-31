#pragma once

#include "../sfml/Joystick.hpp"

namespace SFML {
    struct MovementJoystick {
        MovementJoystick(float speed_value = 1) : speed(speed_value) {}
        float speed;
    };
}