#pragma once

#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../sfml/SpriteManager.hpp"
#include "../../sfml/TextureManager.hpp"
#include "../component/Attack.hpp"
#include "../component/DestroyEntity.hpp"
#include "../component/Direction.hpp"
#include "../component/Speed.hpp"
#include "../../common/component/SpriteReference.hpp"

namespace SFML {
/**
 * @brief System that shoot bullets from the entity that have an Attack Component
 */
class Shoot : public ECS::System {
public:
    /**
     * @brief Function that shoot bullets from the entity that have an Attack Component
     *
     * @param coordinator Reference to the ecs coordinator
     * @param elapsed_time Time in milliseconds you get from the restart member of the clock
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator, std::int32_t elapsed_time) {
        for (const auto &entity: entities) {
            auto &attack = coordinator->getComponent<Attack>(entity);
            auto &entityTransform = coordinator->getComponent<Transform>(entity);
            auto sprite_man = coordinator->getResource<SFML::SpriteManager>();
            auto texture_man = coordinator->getResource<SFML::TextureManager>();
            if (!attack.attack)
                continue;
            attack.attackValue+=elapsed_time+coordinator->getResource<SFML::Clock>()->getElapsedTime().asMilliseconds();
            if (attack.attackValue >= attack.attackSpeed) {
                auto bulletTransform = SFML::Transform(entityTransform.position, 0, { 1.0, 1.0 });
                coordinator->getResource<RType::BulletManager>()->createBullet(coordinator, attack, bulletTransform);
                attack.attackValue = 0;
                if (_bulletNumber == ULLONG_MAX)
                    _bulletNumber = 0;
                else
                    _bulletNumber++;
            }
        }
    }
    /**
     * @brief Number of bullets created from the beginning of the game. Resets to 0 when it overflows.
     */
    unsigned long long int _bulletNumber = 0;
};
}