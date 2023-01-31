#pragma once


#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/TextManager.hpp"
#include "../component/TextReference.hpp"
#include "../component/Tint.hpp"
#include "../sfml/ColorManager.hpp"

namespace SFML {
    class TintText : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            auto text_manager = coordinator.getResource<SFML::TextManager>();
            auto color_manager = coordinator.getResource<SFML::ColorManager>();
            for (const auto &entity : entities) {
                auto &text_ref = coordinator.getComponent<TextReference>(entity);
                auto &tint = coordinator.getComponent<Tint>(entity);
                auto text = text_manager->getText(text_ref.id);
                auto color = color_manager->getColor(tint.id);

                text->setFillColor(color);
            }
        }
    };
}
