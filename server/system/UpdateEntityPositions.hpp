#pragma once

#include "../../common/PackageManager.hpp"
#include "../../common/UDPHandler.hpp"
#include "../../common/packet/EntityPosition.hpp"
#include "../../common/packet/TransformEntity.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../PlayerManager.hpp"
#include "../component/BackupTransform.hpp"
#include <memory>

namespace SFML {
class UpdateEntityPositions : public ECS::System {
public:
    void update(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<RType::Network::UDPHandler>& udp_handler, std::int32_t elapsed_time)
    {
        _lastTime += elapsed_time + coordinator->getResource<SFML::Clock>()->getElapsedTime().asMilliseconds();
        std::cerr << _lastTime << " " << 5000 << std::endl;
        if (_lastTime < 5000)
            return;
        for (const auto& entity : entities) {
            auto& transform = coordinator->getComponent<SFML::Transform>(entity);
            auto& backup_transform = coordinator->getComponent<SFML::BackupTransform>(entity);
            if (!isSameTransform(transform, backup_transform)) {
                RType::Packet::TransformEntity entity_transform(entity, transform);
                std::cout << "Moving from " << backup_transform.position.getVector2().x << " " << backup_transform.position.getVector2().y << " to " << transform.position.getVector2().x << " " << transform.position.getVector2().y << std::endl;
                auto packet = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::TransformEntity>(entity_transform);
                coordinator->getResource<RType::PlayerManager>()->sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
                transformToBackup(transform, backup_transform);
            }
        }
        _lastTime = 0;
    }

private:
    void transformToBackup(SFML::Transform transform, SFML::BackupTransform& backup)
    {
        backup.position = transform.position;
        backup.scale = transform.scale;
        backup.rotation = transform.rotation;
        backup.origin = transform.origin;
    }

    bool isSameTransform(SFML::Transform transform, SFML::BackupTransform backupTransform)
    {
        if (transform.position.getVector2() == backupTransform.position.getVector2()
            && transform.origin.getVector2() == backupTransform.origin.getVector2()
            && transform.rotation == backupTransform.rotation
            && transform.scale.getVector2() == backupTransform.scale.getVector2())
            return true;
        return false;
    }

    std::int32_t _lastTime = 0;
};
}