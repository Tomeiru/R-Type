#pragma once

namespace SFML {
    struct Spin {
        Spin(const float spin_speed = 2) : speed(spin_speed) {};
        float speed;
    };
}