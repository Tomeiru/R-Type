#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../component/SpriteReference.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../sfml/Window.hpp"

namespace SFML {
    class DrawSprite : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            auto sprite_manager = coordinator.getResource<SFML::SpriteManager>();
            auto window = coordinator.getResource<SFML::Window>();
            for (const auto &entity : entities) {
                auto &sprite_ref = coordinator.getComponent<SpriteReference>(entity);
                auto sprite = sprite_manager->getSprite(sprite_ref.id);

                if (sprite->isVisible())
                    window->drawSprite(sprite);
            }
        }
    };
}