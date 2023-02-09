#pragma once

#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../component/DestroyEntity.hpp"

namespace SFML {
/**
 * @brief System that destroys every entity that are out of the window
 */
class DestroyEntityOutWindow : public ECS::System {
public:
    /**
     * @brief Function that destroys every entity that are out of the window
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(ECS::Coordinator &coordinator) {
        std::queue<ECS::Entity> toDestroy;
        for (const auto &entity: entities) {
            auto &destroyStat = coordinator.getComponent<DestroyEntity>(entity);
            auto &transform = coordinator.getComponent<Transform>(entity);
            if (!destroyStat.destroyable)
                continue;
            if (destroyStat.outWindow) {
                if (transform.position.getVector2().x < -1920 || transform.position.getVector2().y < -1080
                    || transform.position.getVector2().x > 3840 || transform.position.getVector2().y > 2160) {
                    if (destroyStat.sprite)
                        std::cout << "TempCode: Destroy Sprite" << std::endl;
                    //TODO destroy sprite
                    toDestroy.push(entity);
                }
            }
        }
        while (!toDestroy.empty()) {
            ECS::Entity tmp = toDestroy.front();
            toDestroy.pop();
            coordinator.destroyEntity(tmp);
        }
    }
};
}