#pragma once

#include <string>

namespace RType::Packet {
    /**
     * @brief PlayerName is a class that represents a player name packet
     */
    struct PlayerName {
        /**
         * @brief Constructs a new PlayerName object
         * @param name The name of the player
         */
        PlayerName(std::string name = "") {
            for (int i = 0; i < 15; i++) {
                _name[i] = (i < name.size() ? name[i] : '\0');
            }
            _name[15] = '\0';
        };
        char _name[16];
    };
}
