#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../component/Hover.hpp"
#include "../component/Tint.hpp"
#include "../component/HoverTint.hpp"

namespace SFML {
    class UpdateHoverTint : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            for (const auto &entity: entities) {
                auto &hover = coordinator.getComponent<Hover>(entity);
                auto &hoverTint = coordinator.getComponent<HoverTint>(entity);
                auto &tint = coordinator.getComponent<Tint>(entity);

                tint.id = hover.hovered ? hoverTint.on_id : hoverTint.off_id;
            }
        }
    };
}
