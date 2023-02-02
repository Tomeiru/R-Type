#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/Mouse.hpp"
#include "../component/Hitbox.hpp"
#include "../component/Clickable.hpp"

namespace SFML {
    /**
     * @brief System that updates the click state of entities with a Clickable component
     */
    class UpdateClick : public ECS::System {
    public:
        /**
         * @brief Function that updates the click state of entities with a Clickable component
         * 
         * @param coordinator Reference to the ecs coordinator
         */
        void update(ECS::Coordinator &coordinator) {
            if (!Mouse::isButtonPressed(Mouse::Left))
                return;
            auto window = coordinator.getResource<SFML::Window>();
            auto mouse_pos = SFML::Mouse::getWindowPosition(window);

            for (const auto &entity: entities) {
                auto &hitbox = coordinator.getComponent<Hitbox>(entity);
                auto &clickable = coordinator.getComponent<Clickable>(entity);

                if (hitbox.rect.contains(mouse_pos.getX(), mouse_pos.getY()))
                    clickable.callback(coordinator, entity);
            }
        }
    };
}
