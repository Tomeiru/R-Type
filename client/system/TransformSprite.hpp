#pragma once

#include "../../common/component/Hitbox.hpp"
#include "../../common/component/SpriteReference.hpp"
#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../../sfml/SpriteManager.hpp"

namespace SFML {
/**
 * @brief System that transforms all sprites with a Transform component
 */
class TransformSprite : public ECS::System {
public:
    /**
     * @brief Function that transforms all sprites with a Transform component
     *
     * @param coordinator Reference to the ecs coordinator
     *
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();
        for (const auto& entity : entities) {
            auto& sprite_ref = coordinator->getComponent<SpriteReference>(entity);
            auto& transform = coordinator->getComponent<Transform>(entity);
            std::cout << "Get transfo" << std::endl;
            std::cout << sprite_ref.id << std::endl;
            auto sprite = sprite_manager->getSprite(sprite_ref.id);

            sprite->setPosition(transform.position);
            sprite->setRotation(transform.rotation);
            sprite->setScale(transform.scale);
            sprite->setOrigin(transform.origin);
            std::cout << "Transform sprite done" << std::endl;
            if (coordinator->hasComponent<Hitbox>(entity)) {
                auto& hitbox = coordinator->getComponent<Hitbox>(entity);
                hitbox.rect = sprite->getGlobalBounds();
            }
        }
        std::cout << "Leave update"<< std::endl;
    }
};
}
