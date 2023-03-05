#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../../sfml/Joystick.hpp"
#include "../component/MovementJoystick.hpp"

namespace SFML {
/**
 * @brief System that updates the position of all entities with a MovementJoystick component and a Transform component
 */
class UpdateJoystickMovements : public ECS::System {
public:
    /**
     * @brief Function that updates the position of all entities with a MovementJoystick component and a Transform component
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        for (const auto& entity : entities) {
            auto& transform = coordinator->getComponent<Transform>(entity);
            auto& movementKeys = coordinator->getComponent<MovementJoystick>(entity);
            auto pos = transform.position.getVector2();

            if (SFML::Joystick::isConnected(0) == true) {
                float x = SFML::Joystick::getAxisPosition(0, SFML::Joystick::Axis::X);
                float y = SFML::Joystick::getAxisPosition(0, SFML::Joystick::Axis::Y);

                if (std::abs(x) >= 20)
                    pos.x += (x / 100) * movementKeys.speed;
                if (std::abs(y) >= 20)
                    pos.y += (y / 100) * movementKeys.speed;
                transform.position = pos;
            }
        }
    }
};
}
