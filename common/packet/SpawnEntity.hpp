#pragma once

#include <string>

namespace RType::Packet {
    /**
     * @brief SpawnEntity is a class that represents a spawn entity packet
     */

    struct SpawnEntity {
        /**
         * @brief Constructs a new SpawnEntity object
         * @param id The id of the entity
         * @param type The type of the entity
         * @param x The x position of the entity
         * @param y The y position of the entity
         */
        SpawnEntity(std::string sprite_id = "", std::string texture_id = 0,float x = 0, float y = 0) :_x(x), _y(y) {
            for (int i = 0; i < 15; i++) {
                _sprite_id[i] = (i < sprite_id.size() ? sprite_id[i] : '\0');
                _texture_id[i] = (i < texture_id.size() ? texture_id[i] : '\0');
            }
            _sprite_id[15] = '\0';
            _texture_id[15] = '\0';
        };

        char _sprite_id[15];
        char _texture_id[15];
        float _x;
        float _y;
    };
};