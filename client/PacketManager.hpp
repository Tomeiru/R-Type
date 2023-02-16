#pragma once

#include <utility>

#include "../common/PackageManager.hpp"
#include "../common/ReceivedPacket.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/packet/DestroyEntity.hpp"
#include "../common/packet/EntityPosition.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../common/packet/TransformEntity.hpp"
#include "../ecs/Coordinator.hpp"
#include "ServerEntityManager.hpp"
namespace RType {
/**
 * @brief Class that manages the received packets and their actions
 */
class PacketManager {
private:
    static void spawnEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager> &package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::SpawnEntity>(packet_received.packet_data);
        auto entity = coordinator->createEntity();
        server_entity_manager->registerServerEntity(packet->_entity, entity);
        coordinator->addComponent<SFML::SpriteReference>(entity, SFML::SpriteReference(packet->_sprite_id));
        coordinator->addComponent<SFML::Transform>(entity, SFML::Transform({ packet->_x, packet->_y }, 0, { 3, 3 }));
    }

    static void entityPosition(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager> &package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::EntityPosition>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        auto& transform = coordinator->getComponent<SFML::Transform>(entity);
        transform.position = SFML::Vector2f { packet->_x, packet->_y };
    }

    static void transformEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager> &package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::TransformEntity>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        auto &transform = coordinator->getComponent<SFML::Transform>(entity);
        transform.position = packet->_position;
        transform.rotation = packet->_rotation;
        transform.scale = packet->_scale;
        transform.origin = packet->_origin;
    }

    static void destroyEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager> &package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::DestroyEntity>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        coordinator->destroyEntity(entity);
    }

public:
    void choosePacket(std::unique_ptr<ECS::Coordinator>& coordinator, char headerId, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager) {
        auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
        if (headerId == package_manager->getTypeId<RType::Packet::SpawnEntity>()) {
            spawnEntity(coordinator, package_manager, packet_received, server_entity_manager);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::EntityPosition>()) {
            entityPosition(coordinator, package_manager, packet_received, server_entity_manager);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::TransformEntity>()) {
            transformEntity(coordinator, package_manager, packet_received, server_entity_manager);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::DestroyEntity>()) {
            destroyEntity(coordinator, package_manager, packet_received, server_entity_manager);
        }
    }
};
}