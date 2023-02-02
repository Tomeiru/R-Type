#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../component/TextReference.hpp"
#include "../sfml/Window.hpp"
#include "../sfml/TextManager.hpp"

namespace SFML {
    /**
     * @brief System that draws all texts
     */
    class DrawText : public ECS::System {
    public:
        /**
         * @brief Function that draws all texts
         * 
         * @param coordinator Reference to the ecs coordinator
         */
        void update(ECS::Coordinator &coordinator) {
            auto text_manager = coordinator.getResource<SFML::TextManager>();
            auto window = coordinator.getResource<SFML::Window>();
            for (const auto &entity : entities) {
                auto &text_ref = coordinator.getComponent<TextReference>(entity);
                auto text = text_manager->getText(text_ref.id);

                if (text->isVisible())
                    window->drawText(text);
            }
        }
    };
}