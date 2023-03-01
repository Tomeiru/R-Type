#pragma once

#include <string>
#include <utility>

namespace SFML {
/**
 * @brief Enum used to define the attack type (its speed, ...)
 */
enum AttackType { NormalAttack,
    FastAttack,
    VeryFastAttack,
    SlowAttack };
/**
 * @brief Component that allows an entity to attack
 */
struct Attack {
    /**
     * @brief Construct a new Attack Component
     *
     * @param attack Does the entity attack
     * @param attackSpeed Attack speed of the entity (will shoot every "attackSpeed" tick of the loop)
     * @param type Type of attack (its speed, ...)
     * @param attackAngle Direction of the bullet shot
     */
    Attack(bool attack = true, unsigned int attackSpeed = 0, AttackType type = AttackType::NormalAttack, unsigned int attackAngle = 180)
        : attack(attack)
        , attackSpeed(attackSpeed)
        , type(type)
        , attackAngle(attackAngle)
        , attackValue(0) {};
    bool attack;
    std::int32_t attackValue;
    unsigned int attackSpeed;
    unsigned int attackAngle;
    AttackType type;
};
}