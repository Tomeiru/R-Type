#pragma once

#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../component/Direction.hpp"
#include "../component/Speed.hpp"
#include <cmath>

namespace SFML {
/**
 * @brief System that move every entity that have a Direction, Speed and Transform Component in a linear way
 */
class LinearMove : public ECS::System {
public:
    /**
     * @brief Function that move every entity that have a Direction, Speed and Transform Component in a linear way
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator) {
        for (const auto &entity: entities) {
            auto &transform = coordinator->getComponent<SFML::Transform>(entity);
            auto speed = coordinator->getComponent<SFML::Speed>(entity).speed;
            auto direction = coordinator->getComponent<SFML::Direction>(entity).angle;
            float rad = direction * M_PI / 180;

            transform.position = {static_cast<float>(transform.position.getVector2().x + std::cos(rad) * speed),static_cast<float>(transform.position.getVector2().y + std::sin(rad) * speed)};
        }
    }
};
}