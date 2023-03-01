#pragma once

#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../../server/component/Direction.hpp"
#include "../../server/component/Speed.hpp"
#include "../../sfml/Clock.hpp"
#include "../component/Transform.hpp"
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
     * @param elapsed_time Time in milliseconds you get from the restart member of the clock
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator, std::int32_t elapsed_time)
    {
        _lastTime += elapsed_time + coordinator->getResource<SFML::Clock>()->getElapsedTime().asMilliseconds();
        if (_lastTime < 50)
            return;
        for (const auto& entity : entities) {
            auto& transform = coordinator->getComponent<SFML::Transform>(entity);
            auto& speed = coordinator->getComponent<SFML::Speed>(entity);
            auto direction = coordinator->getComponent<SFML::Direction>(entity).angle;
            float rad = direction * M_PI / 180;

            transform.position = { static_cast<float>(transform.position.getVector2().x + std::cos(rad) * speed.speed), static_cast<float>(transform.position.getVector2().y + std::sin(rad) * speed.speed) };
        }
        _lastTime = 0;
    }

private:
    std::int32_t _lastTime = 0;
};
}