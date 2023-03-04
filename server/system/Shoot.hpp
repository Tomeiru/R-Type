#pragma once

#include "../../common/component/EntityType.hpp"
#include "../../common/component/SpriteReference.hpp"
#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../sfml/SpriteManager.hpp"
#include "../../sfml/TextureManager.hpp"
#include "../component/Attack.hpp"
#include "../component/DestroyEntity.hpp"
#include "../component/Direction.hpp"
#include "../component/Speed.hpp"

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
    void update(std::unique_ptr<ECS::Coordinator>& coordinator, std::int32_t elapsed_time)
    {
        _lastTime += elapsed_time + coordinator->getResource<SFML::Clock>()->getElapsedTime().asMilliseconds();
        for (const auto& entity : entities) {
            auto& attack = coordinator->getComponent<Attack>(entity);
            auto& entityTransform = coordinator->getComponent<Transform>(entity);
            auto sprite_man = coordinator->getResource<SFML::SpriteManager>();
            auto texture_man = coordinator->getResource<SFML::TextureManager>();
            auto type = coordinator->getComponent<SFML::EntityType>(entity);
            attack.attackValue += _lastTime;
            if (!attack.attack)
                continue;
            // std::cerr << attack.attackValue << " " << attack.attackSpeed << std::endl;
            if (attack.attackValue >= attack.attackSpeed) {
                attack.attackValue = 0;
                auto bulletTransform = SFML::Transform(entityTransform.position, 0, { 1.0, 1.0 });
                std::cerr << "Shooting" << std::endl;
                coordinator->getResource<RType::BulletManager>()->createBullet(coordinator, attack, bulletTransform, type);
                if (_bulletNumber == ULLONG_MAX)
                    _bulletNumber = 0;
                else
                    _bulletNumber++;
            }
        }
        _lastTime = 0;
    }

private:
    unsigned long long int _bulletNumber = 0;
    std::int32_t _lastTime = 0;
};
}