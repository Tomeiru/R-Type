#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/TextManager.hpp"
#include "../component/TextReference.hpp"
#include "../component/Transform.hpp"

namespace SFML {
    class TransformText : public ECS::System {
    public:
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
