#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../component/Hitbox.hpp"
#include "../component/Hover.hpp"
#include "../sfml/Mouse.hpp"

namespace SFML {
    /**
     * @brief System that updates the hover state of entities with a Hover component and a Hitbox component
     */
    class UpdateHover : public ECS::System {
    public:
        /**
         * @brief Function that updates the hover state of entities with a Hover component and a Hitbox component
         * 
         * @param coordinator Reference to the ecs coordinator
         */
        void update(ECS::Coordinator &coordinator) {
            auto window = coordinator.getResource<SFML::Window>();
            auto mouse_pos = SFML::Mouse::getWindowPosition(window);
            for (const auto &entity: entities) {
                auto &hitbox = coordinator.getComponent<Hitbox>(entity);
                auto &hover = coordinator.getComponent<Hover>(entity);

                if (hitbox.rect.contains(mouse_pos.getX(), mouse_pos.getY()))
                    hover.hovered = true;
                else
                    hover.hovered = false;
            }
        }
    };
}
