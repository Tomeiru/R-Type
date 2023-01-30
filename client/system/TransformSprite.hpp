#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../component/SpriteReference.hpp"
#include "../component/Transform.hpp"
#include "../component/Hitbox.hpp"

namespace SFML {
    class TransformSprite : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            auto sprite_manager = coordinator.getResource<SFML::SpriteManager>();
            for (const auto &entity : entities) {
                auto &sprite_ref = coordinator.getComponent<SpriteReference>(entity);
                auto &transform = coordinator.getComponent<Transform>(entity);
                auto sprite = sprite_manager->getSprite(sprite_ref.id);

                sprite->setPosition(transform.position);
                sprite->setRotation(transform.rotation);
                sprite->setScale(transform.scale);
                sprite->setOrigin(transform.origin);
                if (coordinator.hasComponent<Hitbox>(entity)) {
                    auto &hitbox = coordinator.getComponent<Hitbox>(entity);
                    hitbox.rect = sprite->getGlobalBounds();
                }
            }
        }
    };
}
