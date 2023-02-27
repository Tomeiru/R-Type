#pragma once

#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../../sfml/ColorManager.hpp"
#include "../../sfml/TextManager.hpp"
#include "../component/Outline.hpp"
#include "../component/TextReference.hpp"

namespace SFML {
/**
 * @brief System that sets the outline of all texts
 */
class OutlineText : public ECS::System {
public:
    /**
     * @brief Function that sets the outline of all texts
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        auto text_manager = coordinator->getResource<SFML::TextManager>();
        auto color_manager = coordinator->getResource<SFML::ColorManager>();
        for (const auto& entity : entities) {
            auto& text_ref = coordinator->getComponent<TextReference>(entity);
            auto& outline = coordinator->getComponent<Outline>(entity);
            auto text = text_manager->getText(text_ref.id);
            auto color = color_manager->getColor(outline.id);

            text->setOutlineColor(color);
            text->setOutlineThickness(outline.thickness);
        }
    }
};
}
