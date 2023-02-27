#pragma once

#include "../component/Spin.hpp"
#include "../component/SpriteReference.hpp"
#include "../component/Tint.hpp"
#include "../component/Transform.hpp"
#include "../ecs/Coordinator.hpp"
#include "../ecs/System.hpp"
#include "../sfml/ColorManager.hpp"
#include "../sfml/SpriteManager.hpp"

namespace SFML {
/**
 * @brief System that spins all sprites with a Spin component and a Transform component
 */
class SpinSprite : public ECS::System {
public:
    /**
     * @brief Function that spins all sprites with a Spin component and a Transform component
     *
     * @param coordinator Reference to the ecs coordinator
     *
     */
    void update(ECS::Coordinator& coordinator)
    {
        for (const auto& entity : entities) {
            std::cout << "Entity: " << entity << std::endl;
            auto& transform = coordinator.getComponent<Transform>(entity);
            auto& spin = coordinator.getComponent<Spin>(entity);
            transform.rotation += spin.speed;
        }
    }
};
}
