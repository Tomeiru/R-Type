#pragma once

#include <cstdint>

namespace RType {
using BulletID = std::uint64_t;
using EnemyID = std::uint64_t;
using PlayerID = std::uint16_t;
using PlayerName = std::string;

/**
 * @brief Attack parameters of an enemy
 */
struct Enemy_Attack_s {
    unsigned int speed;
    unsigned int bullet_speed;
    unsigned int dir;
};

/**
 * @brief Transform parameters of an enemy
 */
struct Enemy_Transform_s {
    float x;
    float y;
    unsigned int rotation;
    unsigned int scale;
};

using Enemy_Attack = Enemy_Attack_s;
using Enemy_Transform = Enemy_Transform_s;

/**
 * @brief
 * @param attack Enemy's attack informations
 * @param dir Enemy's direction
 * @param speed Enemy's speed
 * @param life Enemy's life
 * @param texture_ref Enemy's texture reference
 * @param transform Enemy's transorm informations
 */
struct Enemy_s {
    Enemy_Attack attack;
    unsigned int dir;
    unsigned int speed;
    unsigned int life;
    const char* texture_ref;
    Enemy_Transform transform;
};
using Enemy = Enemy_s;
} // namespace RType