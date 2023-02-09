#pragma once

#include "../../common/component/Tint.hpp"
#include "../component/Hover.hpp"
#include "../component/HoverTint.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"

namespace SFML {
/**
 * @brief System that updates the tint of all entities with a HoverTint component, a Hover component and a Tint component
 */
class UpdateHoverTint : public ECS::System {
public:
    /**
     * @brief Function that updates the tint of all entities with a HoverTint component, a Hover component and a Tint component
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        for (const auto& entity : entities) {
            auto& hover = coordinator->getComponent<Hover>(entity);
            auto& hoverTint = coordinator->getComponent<HoverTint>(entity);
            auto& tint = coordinator->getComponent<Tint>(entity);

            tint.id = hover.hovered ? hoverTint.on_id : hoverTint.off_id;
        }
    }
};
}
