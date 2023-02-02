#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../component/SpriteReference.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../sfml/Window.hpp"

namespace SFML {
    /**
     * @brief System that draws all sprites
     */
    class DrawSprite : public ECS::System {
    public:
        /**
         * @brief Function that draws all sprites
         * 
         * @param coordinator Reference to the ecs coordinator
         */
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