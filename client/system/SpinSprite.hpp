#pragma once

#include "../../common/component/SpriteReference.hpp"
#include "../../common/component/Tint.hpp"
#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../../sfml/ColorManager.hpp"
#include "../../sfml/SpriteManager.hpp"
#include "../component/Spin.hpp"

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
    void update(std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        for (const auto& entity : entities) {
            std::cout << "Entity: " << entity << std::endl;
            auto& transform = coordinator->getComponent<Transform>(entity);
            auto& spin = coordinator->getComponent<Spin>(entity);
            transform.rotation += spin.speed;
        }
    }
};
}
