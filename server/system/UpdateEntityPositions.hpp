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
/**
 * @brief System to update entity transform if it changed
 */
class UpdateEntityPositions : public ECS::System {
public:
    /**
     * @brief Function that send a packet to all player to call them to update the transform if it changed from the last call of this function
     *
     * @param coordinator Reference to the ecs coordinator
     * @param udp_handler The class that handles sending packet to other
     * UDPClients
     * @param elapsed_time Time in milliseconds you get from the restart member of the clock
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<RType::Network::UDPHandler>& udp_handler, std::int32_t elapsed_time)
    {
        _lastTime += elapsed_time;
        //std::cerr << _lastTime << " " << 5000 << std::endl;
        if (_lastTime < 200)
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
    /**
     * @brief Function that copy the transform
     *
     * @param transform Transform component to copy
     * @param backup Backup component to copy to
     */
    void transformToBackup(SFML::Transform transform, SFML::BackupTransform& backup)
    {
        backup.position = transform.position;
        backup.scale = transform.scale;
        backup.rotation = transform.rotation;
        backup.origin = transform.origin;
    }

    /**
     * @brief Function that checks if two transform components are the same
     *
     * @param transform Transform component to check
     * @param backupTransform Backup of a transform to check
     * @return True if both transforms are the same, false otherwise
     */
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