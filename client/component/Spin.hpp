#pragma once

namespace SFML {
    /**
     * @brief Component that allows an entity to spin
     */
    struct Spin {
        /**
         * @brief Construct a new Spin Component
         * 
         * @param spin_speed Speed of the spin
         */
        Spin(const float spin_speed = 2) : speed(spin_speed) {};
        float speed;
    };
}