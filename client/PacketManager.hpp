#pragma once

#include <utility>

#include "../common/PackageManager.hpp"
#include "../common/ReceivedPacket.hpp"
#include "../common/component/EntityType.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/packet/DestroyEntity.hpp"
#include "../common/packet/EntityPosition.hpp"
#include "../common/packet/SetEntityLinearMove.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../common/packet/TransformEntity.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/SoundBufferManager.hpp"
#include "../sfml/SoundManager.hpp"
#include "./component/SoundReference.hpp"
#include "./system/PlaySound.hpp"
#include "ServerEntityManager.hpp"
namespace RType {
/**
 * @brief Class that manages the received packets and their actions
 */
class PacketManager {
private:
    /**
     * @brief Function that receive the SpawnEntity packet and manage it
     *
     * @param coordinator Reference to the ECS coordinator
     * @param package_manager Package manager that is used to deal with received packet
     * @param packet_received Packet that got received (not casted)
     * @param server_entity_manager Manager dealing with the server to find the great entity
     */
    static void spawnEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::SpawnEntity>(packet_received.packet_data);
        auto entity = coordinator->createEntity();
        server_entity_manager->registerServerEntity(packet->_entity, entity);
        coordinator->addComponent<SFML::SpriteReference>(entity, SFML::SpriteReference(packet->_sprite_id));
        coordinator->addComponent<SFML::Transform>(entity, SFML::Transform({ packet->_x, packet->_y }, 0, { 3, 3 }));
        if (packet->_type.type == SFML::EntityTypeEnum::Player) {
            auto player_bullet = coordinator->createEntity();
            coordinator->addComponent(player_bullet, SFML::SoundReference("player_bullet", SFML::Sound::Playing));
        }
    }

    /**
     * @brief Function that receive the EntityPosition packet and manage it
     *
     * @param coordinator Reference to the ECS coordinator
     * @param package_manager Package manager that is used to deal with received packet
     * @param packet_received Packet that got received (not casted)
     * @param server_entity_manager Manager dealing with the server to find the great entity
     */
    static void entityPosition(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::EntityPosition>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        auto& transform = coordinator->getComponent<SFML::Transform>(entity);
        transform.position = SFML::Vector2f { packet->_x, packet->_y };
    }

    /**
     * @brief Function that receive the TransformEntity packet and manage it
     *
     * @param coordinator Reference to the ECS coordinator
     * @param package_manager Package manager that is used to deal with received packet
     * @param packet_received Packet that got received (not casted)
     * @param server_entity_manager Manager dealing with the server to find the great entity
     */
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

    /**
     * @brief Function that receive the DestroyEntity packet and manage it
     *
     * @param coordinator Reference to the ECS coordinator
     * @param package_manager Package manager that is used to deal with received packet
     * @param packet_received Packet that got received (not casted)
     * @param server_entity_manager Manager dealing with the server to find the great entity
     */
    static void destroyEntity(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
    {
        auto packet = package_manager->decodeContent<RType::Packet::DestroyEntity>(packet_received.packet_data);
        auto entity = server_entity_manager->getClientEntity(packet->_entity);
        coordinator->destroyEntity(entity);
    }

    /**
     * @brief Function that receive the CreateSpriteReference packet and manage it
     *
     * @param coordinator Reference to the ECS coordinator
     * @param package_manager Package manager that is used to deal with received packet
     * @param packet_received Packet that got received (not casted)
     */
    static void createSpriteReference(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received)
    {
        auto texture_manager = coordinator->getResource<SFML::TextureManager>();
        auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();
        auto packet = package_manager->decodeContent<RType::Packet::CreateSpriteReference>(packet_received.packet_data);
        sprite_manager->registerSprite(packet->_spriteId, texture_manager->getTexture(packet->_linkSprite));
    }

    /**
     * @brief Function that receive the SetEntityLinearMove packet and manages it
     *
     * @param coordinator Reference to the ECS coordinator
     * @param package_manager Package manager that is used to deal with received packet
     * @param packet_received Packet that got received (not casted)
     * @param server_entity_manager Manager dealing with the server to find the great entity
     */
    static void setEntityLinearMove(std::unique_ptr<ECS::Coordinator>& coordinator, std::shared_ptr<Network::PackageManager>& package_manager, const RType::Network::ReceivedPacket& packet_received, const std::shared_ptr<RType::Client::ServerEntityManager>& server_entity_manager)
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
    /**
     * @brief Function that choose the right packet
     *
     * @param coordinator Reference to the ECS coordinator
     * @param headerId Char that corresponds to a packet type
     * @param packet_received Packet that got received (not casted)
     * @param server_entity_manager Manager dealing with the server to find the great entity
     */
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
            std::cerr << "Crash after setEntityLinearMove ?" << std::endl;
        }
    }
};
}