#pragma once

#include <utility>

#include "../common/PackageManager.hpp"
#include "../common/ReceivedPacket.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/packet/DestroyEntity.hpp"
#include "../common/packet/EntityPosition.hpp"
#include "../common/packet/SetEntityLinearMove.hpp"
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
    static void spawnEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::SpawnEntity>(packet_received.packet_data);
        auto entity = coordinator->createEntity();
        server_entity_manager->registerServerEntity(packet->_entity, entity);
        coordinator->addComponent<SFML::SpriteReference>(entity, SFML::SpriteReference(packet->_sprite_id));
        coordinator->addComponent<SFML::Transform>(entity, SFML::Transform({ packet->_x, packet->_y }, 0, { 3, 3 }));
    }

    static void entityPosition(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::EntityPosition>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        auto& transform = coordinator->getComponent<SFML::Transform>(entity);
        transform.position = SFML::Vector2f { packet->_x, packet->_y };
    }

    static void transformEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::TransformEntity>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        auto& transform = coordinator->getComponent<SFML::Transform>(entity);
        transform.position = packet->_position;
        transform.rotation = packet->_rotation;
        transform.scale = packet->_scale;
        transform.origin = packet->_origin;
    }

    static void destroyEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::DestroyEntity>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        coordinator->destroyEntity(entity);
    }

    static void createSpriteReference(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager> &package_manager, const RType::Network::ReceivedPacket& packet_received)
    {
        auto texture_manager = coordinator->getResource<SFML::TextureManager>();
        auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();
        auto packet = package_manager->decodeContent<RType::Packet::CreateSpriteReference>(packet_received.packet_data);
        sprite_manager->registerSprite(packet->_spriteId, texture_manager->getTexture(packet->_linkSprite));
    }

    static void setEntityLinearMove(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager> &package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::SetEntityLinearMove>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        if (packet->_newComps) {
            coordinator->addComponent(entity, SFML::Speed(packet->_speed));
            coordinator->addComponent(entity, SFML::Direction(packet->_direction));
        } else {
            coordinator->getComponent<SFML::Speed>(entity).speed = packet->_speed;
            coordinator->getComponent<SFML::Direction>(entity).angle = packet->_direction;
        }
    }

public:
    void choosePacket(std::unique_ptr<ECS::Coordinator>& coordinator, char headerId, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
        if (headerId == package_manager->getTypeId<RType::Packet::SpawnEntity>()) {
            std::cerr << "It's a SpawnEntity packet" << std::endl;
            spawnEntity(coordinator, package_manager, packet_received, server_entity_manager);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::EntityPosition>()) {
            std::cerr << "It's a EntityPosition packet" << std::endl;
            entityPosition(coordinator, package_manager, packet_received, server_entity_manager);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::TransformEntity>()) {
            std::cerr << "It's a TransformEntity packet" << std::endl;
            transformEntity(coordinator, package_manager, packet_received, server_entity_manager);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::DestroyEntity>()) {
            std::cerr << "It's a DestroyEntity packet" << std::endl;
            destroyEntity(coordinator, package_manager, packet_received, server_entity_manager);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::CreateSpriteReference>()) {
            std::cerr << "It's a CreateSpriteReference packet" << std::endl;
            createSpriteReference(coordinator, package_manager, packet_received);
        }
        if (headerId == package_manager->getTypeId<RType::Packet::SetEntityLinearMove>()) {
            std::cerr << "It's a SetEntityLinearMove packet" << std::endl;
            setEntityLinearMove(coordinator, package_manager, packet_received, server_entity_manager);
        }
    }
};
}