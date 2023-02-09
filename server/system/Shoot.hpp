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
private:
    /**
     * @brief Function that sets the speed of a bullet
     *
     * @param coordinator Reference to the ecs coordinator
     * @param bullet Bullet entity to speed up
     * @param type Type of bullet to shoot
     */
    void setBulletSpeed(ECS::Coordinator &coordinator, ECS::Entity bullet, SFML::AttackType type) {
        switch (type) {
        case SFML::AttackType::NormalAttack:
            coordinator.addComponent(bullet, SFML::Speed(2));
            break;
        case SFML::AttackType::FastAttack:
            coordinator.addComponent(bullet, SFML::Speed(4));
            break;
        case SFML::AttackType::VeryFastAttack:
            coordinator.addComponent(bullet, SFML::Speed(3));
            break;
        case SFML::AttackType::SlowAttack:
            coordinator.addComponent(bullet, SFML::Speed(1));
            break;
        default:
            coordinator.addComponent(bullet, SFML::Speed(2));
            break;
        }
    }
public:
    /**
     * @brief Function that shoot bullets from the entity that have an Attack Component
     *
     * @param coordinator Reference to the ecs coordinator
     * @param sprite_man Pointer to the sprite manager that contains every sprite (so you can add one for each bullet)
     * @param texture_man Pointer to the texture manager that contains every texture in the game (so you can choose the texture of your bullet)
     */
    void update(ECS::Coordinator &coordinator, const std::shared_ptr<SpriteManager>& sprite_man, const std::shared_ptr<TextureManager>& texture_man) {
        for (const auto &entity: entities) {
            auto &attack = coordinator.getComponent<Attack>(entity);
            auto &entityTransform = coordinator.getComponent<Transform>(entity);
            if (!attack.attack)
                continue;
            if (attack.attackValue >= attack.attackSpeed) {
                auto bullet = coordinator.createEntity();
                std::string bulletId = "bullet"+std::to_string(_bulletNumber);

                sprite_man->registerSprite(bulletId, texture_man->getTexture("enemy"));
                coordinator.addComponent(bullet, SFML::Direction(attack.attackAngle));
                coordinator.addComponent(bullet, SFML::SpriteReference(bulletId));
                coordinator.addComponent(bullet, SFML::Transform({entityTransform.position.getVector2().x, entityTransform.position.getVector2().y}, 0, {0.1, 0.1}));
                setBulletSpeed(coordinator, bullet, attack.type);
                coordinator.addComponent(bullet, SFML::DestroyEntity(true, true, true));
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