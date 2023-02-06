#pragma once

#include "../component/SpriteReference.hpp"
#include "../component/Tint.hpp"
#include "../ecs/Coordinator.hpp"
#include "../ecs/System.hpp"
#include "../sfml/ColorManager.hpp"
#include "../sfml/SpriteManager.hpp"

namespace SFML {
/**
 * @brief System that tints all sprites with a Tint component and a SpriteReference component
 */
class TintSprite : public ECS::System {
public:
    /**
     * @brief Function that tints all sprites with a Tint component
     *
     * @param coordinator Reference to the ecs coordinator
     *
     */
    void update(ECS::Coordinator& coordinator)
    {
        auto sprite_manager = coordinator.getResource<SFML::SpriteManager>();
        auto color_manager = coordinator.getResource<SFML::ColorManager>();
        for (const auto& entity : entities) {
            auto& sprite_ref = coordinator.getComponent<SpriteReference>(entity);
            auto& tint = coordinator.getComponent<Tint>(entity);
            auto sprite = sprite_manager->getSprite(sprite_ref.id);
            auto color = color_manager->getColor(tint.id);

            sprite->setColor(color);
        }
    }
};
}
