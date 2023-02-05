#pragma once

#include "../../sfml/Keyboard.hpp"

namespace SFML {
    /**
     * @brief Component that allows an entity to be moved with the keyboard
     */
    struct InputKeys {
        /**
         * @brief Construct a new InputKeys Component
         * 
         * @param speed_value Speed of the entity
         * @param key_up Key to move the entity up
         * @param key_right Key to move the entity right
         * @param key_down Key to move the entity down
         * @param key_left Key to move the entity left
         */
        InputKeys(SFML::Keyboard::Key key_up = SFML::Keyboard::Key::Z, SFML::Keyboard::Key key_right = SFML::Keyboard::Key::D,
                  SFML::Keyboard::Key key_down = SFML::Keyboard::Key::S, SFML::Keyboard::Key key_left = SFML::Keyboard::Key::Q, SFML::Keyboard::Key key_shoot = SFML::Keyboard::Key::Space) :
        up(key_up), right(key_right), down(key_down), left(key_left), shoot(key_shoot), key_down_pressed(false), key_up_pressed(false), key_left_pressed(false), key_right_pressed(false), key_shoot_pressed(false) {}
        SFML::Keyboard::Key up;
        SFML::Keyboard::Key right;
        SFML::Keyboard::Key down;
        SFML::Keyboard::Key left;
        SFML::Keyboard::Key shoot;
        bool key_down_pressed;
        bool key_up_pressed;
        bool key_left_pressed;
        bool key_right_pressed;
        bool key_shoot_pressed;
    };
}