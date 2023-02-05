#include "../../ecs/System.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../sfml/Keyboard.hpp"
#include "../../common/component/Transform.hpp"
#include "../component/MovementKeys.hpp"

namespace SFML {
    /**
     * @brief System that updates the position of all entities with a MovementKeys component and a Transform component
     */
    class UpdateKeysMovements : public ECS::System {
    public:
        /**
         * @brief Function that updates the position of all entities with a MovementKeys component and a Transform component
         * 
         * @param coordinator Reference to the ecs coordinator
         */
        void update(ECS::Coordinator &coordinator) {
            for (const auto &entity: entities) {
                auto &movementKeys = coordinator.getComponent<MovementKeys>(entity);

                movementKeys.key_up_pressed = SFML::Keyboard::isKeyPressed(movementKeys.up);
                movementKeys.key_down_pressed = SFML::Keyboard::isKeyPressed(movementKeys.down);
                movementKeys.key_left_pressed = SFML::Keyboard::isKeyPressed(movementKeys.left);
                movementKeys.key_right_pressed = SFML::Keyboard::isKeyPressed(movementKeys.right);
            }
        }
    };
}
