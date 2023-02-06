#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/TextManager.hpp"
#include "../component/TextReference.hpp"
#include "../component/Transform.hpp"

namespace SFML {
    /**
     * @brief System that sets the transform of all texts with a Transform component
     */
    class TransformText : public ECS::System {
    public:
        /**
         * @brief Function that sets the transform of all texts with a Transform component
         * 
         * @param coordinator Reference to the ecs coordinator
         */
        void update(ECS::Coordinator &coordinator) {
            auto text_manager = coordinator.getResource<SFML::TextManager>();
            for (const auto &entity : entities) {
                auto &text_ref = coordinator.getComponent<TextReference>(entity);
                auto &transform = coordinator.getComponent<Transform>(entity);
                auto text = text_manager->getText(text_ref.id);

                text->setPosition(transform.position);
                text->setRotation(transform.rotation);
                text->setScale(transform.scale);
                text->setOrigin(transform.origin);
            }
        }
    };
}
