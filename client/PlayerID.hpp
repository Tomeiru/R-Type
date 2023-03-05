#pragma once

#include <cstdint>

namespace RType {
namespace Client {
/**
 * @brief ID of the player, used to send packets to the server
 */
struct PlayerID {
    /**
     * @brief Construct a new PlayerID object
     *
     * @param id ID of the current player
     */
    PlayerID(std::uint16_t id = 0)
        : id(id) {};
    std::uint16_t id;
};
} // RType
} // Client
