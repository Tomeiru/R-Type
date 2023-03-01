#pragma once

namespace RType::Packet {
/**
 * @brief GameStart is a class that represents a game start packet
 */
struct GameStart {
    /**
     * @brief Constructs a new GameStart packet
     *
     * @param player_id The id of the player
     */
    GameStart(std::uint16_t player_id = 0)
        : player_id(player_id) {};
    std::uint16_t player_id;
};
}