#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/TextManager.hpp"
#include "../sfml/ColorManager.hpp"
#include "../component/TextReference.hpp"
#include "../component/Outline.hpp"

namespace SFML {
    class OutlineText : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            auto text_manager = coordinator.getResource<SFML::TextManager>();
            auto color_manager = coordinator.getResource<SFML::ColorManager>();
            for (const auto &entity : entities) {
                auto &text_ref = coordinator.getComponent<TextReference>(entity);
                auto &outline = coordinator.getComponent<Outline>(entity);
                auto text = text_manager->getText(text_ref.id);
                auto color = color_manager->getColor(outline.id);

                text->setOutlineColor(color);
                text->setOutlineThickness(outline.thickness);
            }
        }
    };
}
