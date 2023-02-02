#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/Keyboard.hpp"
#include "../component/Transform.hpp"
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
                float x_movements = 0;
                float y_movements = 0;
                auto &transform = coordinator.getComponent<Transform>(entity);
                auto &movementKeys = coordinator.getComponent<MovementKeys>(entity);
                auto pos = transform.position.getVector2();
    
                if (SFML::Keyboard::isKeyPressed(movementKeys.up))
                    y_movements -= 1;
                if (SFML::Keyboard::isKeyPressed(movementKeys.down))
                    y_movements += 1;
                if (SFML::Keyboard::isKeyPressed(movementKeys.left))
                    x_movements -= 1;
                if (SFML::Keyboard::isKeyPressed(movementKeys.right))
                    x_movements += 1;
                
                if (x_movements != 0 || y_movements != 0)
                    transform.position = {pos.x + x_movements * movementKeys.speed, pos.y + y_movements * movementKeys.speed};
            }
        }
    };
}
