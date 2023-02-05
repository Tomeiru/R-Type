#include "../../ecs/System.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../sfml/Keyboard.hpp"
#include "../../common/component/Transform.hpp"
#include "../component/InputKeys.hpp"

namespace SFML {
    /**
     * @brief System that updates the position of all entities with a InputKeys component and a Transform component
     */
    class UpdateKeysInput : public ECS::System {
    public:
        /**
         * @brief Function that updates the position of all entities with a InputKeys component and a Transform component
         * 
         * @param coordinator Reference to the ecs coordinator
         */
        void update(std::unique_ptr<ECS::Coordinator> &coordinator) {
            for (const auto &entity: entities) {
                auto &input_keys = coordinator->getComponent<InputKeys>(entity);

                input_keys.key_up_pressed = SFML::Keyboard::isKeyPressed(input_keys.up);
                input_keys.key_down_pressed = SFML::Keyboard::isKeyPressed(input_keys.down);
                input_keys.key_left_pressed = SFML::Keyboard::isKeyPressed(input_keys.left);
                input_keys.key_right_pressed = SFML::Keyboard::isKeyPressed(input_keys.right);
                input_keys.key_shoot_pressed = SFML::Keyboard::isKeyPressed(input_keys.shoot);
            }
        }
    };
}
