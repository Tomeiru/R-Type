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
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator) {
        for (const auto &entity: entities) {
            auto &attack = coordinator->getComponent<Attack>(entity);
            auto &entityTransform = coordinator->getComponent<Transform>(entity);
            auto sprite_man = coordinator->getResource<SFML::SpriteManager>();
            auto texture_man = coordinator->getResource<SFML::TextureManager>();
            if (!attack.attack)
                continue;
            if (attack.attackValue >= attack.attackSpeed) {
                //TODO Call bulletmanager
                attack.attackValue = 0;
                if (_bulletNumber == ULLONG_MAX)
                    _bulletNumber = 0;
                else
                    _bulletNumber++;
            }
            attack.attackValue+=1;
        }
    }
    /**
     * @brief Number of bullets created from the beginning of the game. Resets to 0 when it overflows.
     */
    unsigned long long int _bulletNumber = 0;
};
}