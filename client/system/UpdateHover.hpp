#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../component/Hitbox.hpp"
#include "../component/Hover.hpp"
#include "../sfml/Mouse.hpp"

namespace SFML {
    class UpdateHover : public ECS::System {
    public:
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
