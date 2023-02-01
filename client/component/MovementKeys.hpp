#pragma once

#include "../sfml/Keyboard.hpp"

namespace SFML {
    /**
     * @brief Component that allows an entity to be moved with the keyboard
     */
    struct MovementKeys {
        /**
         * @brief Construct a new MovementKeys Component
         * 
         * @param speed_value Speed of the entity
         * @param key_up Key to move the entity up
         * @param key_right Key to move the entity right
         * @param key_down Key to move the entity down
         * @param key_left Key to move the entity left
         */
        MovementKeys(float speed_value = 1, SFML::Keyboard::Key key_up = SFML::Keyboard::Key::Z, SFML::Keyboard::Key key_right = SFML::Keyboard::Key::D, 
        SFML::Keyboard::Key key_down = SFML::Keyboard::Key::S, SFML::Keyboard::Key key_left = SFML::Keyboard::Key::Q) : up(key_up), right(key_right), down(key_down), left(key_left), speed(speed_value) {}
        SFML::Keyboard::Key up;
        SFML::Keyboard::Key right;
        SFML::Keyboard::Key down;
        SFML::Keyboard::Key left;
        float speed;
    };
}