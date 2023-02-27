#pragma once

#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../component/Health.hpp"

namespace SFML {
class KillNoLife : public ECS::System {
public:
    void update(std::unique_ptr<ECS::Coordinator>& coordinator) {
        std::queue<ECS::Entity> toKill;
        for (const auto &entity : entities) {
            auto health = coordinator->getComponent<SFML::Health>(entity).healthPoints;

            if (health <= 0)
                toKill.push(entity);
        }
        while (!toKill.empty()) {
            ECS::Entity tmp = toKill.front();
            toKill.pop();
            coordinator->destroyEntity(tmp);
            RType::Packet::DestroyEntity entity_destroy(tmp);
            auto packet = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::DestroyEntity>(entity_destroy);
            auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
            coordinator->getResource<RType::PlayerManager>()->sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
        }
    }
};
}