#pragma once


#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../sfml/ColorManager.hpp"
#include "../component/SpriteReference.hpp"
#include "../component/Tint.hpp"

namespace SFML {
    class TintSprite : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            auto sprite_manager = coordinator.getResource<SFML::SpriteManager>();
            auto color_manager = coordinator.getResource<SFML::ColorManager>();
            for (const auto &entity : entities) {
                auto &sprite_ref = coordinator.getComponent<SpriteReference>(entity);
                auto &tint = coordinator.getComponent<Tint>(entity);
                auto sprite = sprite_manager->getSprite(sprite_ref.id);
                auto color = color_manager->getColor(tint.id);

                sprite->setColor(color);
            }
        }
    };
}
