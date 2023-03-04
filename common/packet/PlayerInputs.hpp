#pragma once

namespace RType::Packet {
/**
 * @brief PlayerInputs is a class that represents a player inputs packet
 */
struct PlayerInputs {
    /**
     * @brief Constructs a new PlayerInputs object
     *
     * @param left The left input
     * @param right The right input
     * @param up The up input
     * @param down The down input
     * @param shoot The shoot input
     */
    PlayerInputs(char left = false, char right = false, char up = false, char down = false, char shoot = false)
        : left(left)
        , right(right)
        , up(up)
        , down(down)
        , shoot(shoot) {};

    char left;
    char right;
    char up;
    char down;
    char shoot;
};
};