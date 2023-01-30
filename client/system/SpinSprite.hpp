#pragma once


#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../sfml/ColorManager.hpp"
#include "../component/SpriteReference.hpp"
#include "../component/Tint.hpp"
#include "../component/Spin.hpp"
#include "../component/Transform.hpp"

namespace SFML {
    class SpinSprite : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            for (const auto &entity : entities) {
                std::cout << "Entity: " << entity << std::endl;
                auto &transform = coordinator.getComponent<Transform>(entity);
                auto &spin = coordinator.getComponent<Spin>(entity);
                transform.rotation += spin.speed;
            }
        }
    };
}
