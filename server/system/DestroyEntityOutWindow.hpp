#pragma once

#include "../../common/component/Transform.hpp"
#include "../../common/packet/DestroyEntity.hpp"
#include "../../common/UDPHandler.hpp"
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
    void update(std::unique_ptr<ECS::Coordinator>& coordinator) {
        std::queue<std::pair<ECS::Entity, bool>> toDestroy;
        for (const auto &entity: entities) {
            auto &destroyStat = coordinator->getComponent<SFML::DestroyEntity>(entity);
            auto &transform = coordinator->getComponent<SFML::Transform>(entity);
            if (!destroyStat.destroyable)
                continue;
            if (destroyStat.outWindow) {
                if (transform.position.getVector2().x < -1920 || transform.position.getVector2().y < -1080
                    || transform.position.getVector2().x > 3840 || transform.position.getVector2().y > 2160) {
                    toDestroy.push(std::pair(entity, destroyStat.sprite));
                }
            }
        }
        while (!toDestroy.empty()) {
            std::pair<ECS::Entity, bool> tmp = toDestroy.front();
            toDestroy.pop();
            coordinator->destroyEntity(tmp.first);
            RType::Packet::DestroyEntity entity_destroy(tmp.first);
            auto packet = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::DestroyEntity>(entity_destroy);
            auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
            coordinator->getResource<RType::PlayerManager>()->sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
            if (tmp.second)
                std::cout << "TempCode: Destroy the sprite" << std::endl;
            //TODO Destroy sprite
        }
    }
};
}