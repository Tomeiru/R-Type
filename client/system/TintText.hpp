#pragma once

#include "../../common/component/Tint.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../../sfml/ColorManager.hpp"
#include "../../sfml/TextManager.hpp"
#include "../component/TextReference.hpp"

namespace SFML {
/**
 * @brief System that sets the tint of all texts with a Tint component
 */
class TintText : public ECS::System {
public:
    /**
     * @brief Function that tint all texts with a color
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        auto text_manager = coordinator->getResource<SFML::TextManager>();
        auto color_manager = coordinator->getResource<SFML::ColorManager>();
        for (const auto& entity : entities) {
            auto& text_ref = coordinator->getComponent<TextReference>(entity);
            auto& tint = coordinator->getComponent<Tint>(entity);
            auto text = text_manager->getText(text_ref.id);
            auto color = color_manager->getColor(tint.id);

            text->setFillColor(color);
        }
    }
};
}
