#pragma once

#include <string>

namespace RType::Packet {
    /**
     * @brief EntityPosition is a class that represents an enemy position packet
     */
    struct EntityPosition {

        EntityPosition(std::string id = "", float x = 0, float y = 0) : _x(x), _y(y) {
            for (int i = 0; i < 15; i++) {
                sprite_id[i] = (i < id.size() ? id[i] : '\0');
            }
            sprite_id[15] = '\0';
        };

        char sprite_id[16];
        float _x;
        float _y;
    };
}