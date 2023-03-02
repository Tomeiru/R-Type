#pragma once

#include "../common/UDPHandler.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../common/packet/TransformEntity.hpp"
#include "../ecs/Coordinator.hpp"
#include "../ecs/Types.hpp"
#include "MapManager.hpp"
#include "Types.hpp"
#include "component/Attack.hpp"
#include "component/BackupTransform.hpp"
#include "component/DestroyEntity.hpp"
#include "component/Direction.hpp"
#include "component/Health.hpp"
#include "component/Speed.hpp"
#include <memory>
#include <unordered_map>

namespace RType {
/**
 * @brief EnemyManager is the container that handles enemy registration
 */
class EnemyManager {
public:
    /**
     * @brief Constructs a new EnemyManager object
     */
    EnemyManager()
        : enemy_nbr(0) {};

    /**
     * @brief Spawn a new enemy on the game
     * @param udp_handler The class that handles sending packet to other
     * UDPClients
     * @param coordinator Reference to the ecs coordinator
     * @param id Id of the enemy you want to spawn (json)
     * @return ID of the enemy that you spawned
     */
    EnemyID spawnEnemy(std::shared_ptr<Network::UDPHandler>& udp_handler, std::unique_ptr<ECS::Coordinator>& coordinator, std::uint64_t id)
    {
        if (enemy_nbr == UINT64_MAX)
            return 0;
        MapManager mapManager;
        std::string name = "enemy_" + std::to_string(enemy_nbr);
        auto enemy = coordinator->createEntity();
        coordinator->addComponent(enemy, SFML::Transform({ mapManager.getEnemyInfosById(id).transform.x, mapManager.getEnemyInfosById(id).transform.y }, mapManager.getEnemyInfosById(id).transform.rotation, { 4, 4 }));
        coordinator->addComponent(enemy, SFML::BackupTransform(coordinator->getComponent<SFML::Transform>(enemy)));
        coordinator->addComponent(enemy, SFML::SpriteReference(name));
        coordinator->addComponent(enemy, SFML::Speed(mapManager.getEnemyInfosById(id).speed));
        coordinator->addComponent(enemy, SFML::Direction(mapManager.getEnemyInfosById(id).dir));
        coordinator->addComponent(enemy, SFML::Attack(true, mapManager.getEnemyInfosById(id).attack.speed, static_cast<SFML::AttackType>(mapManager.getEnemyInfosById(id).attack.bullet_speed), mapManager.getEnemyInfosById(id).attack.dir));
        coordinator->addComponent(enemy, SFML::DestroyEntity(true, true, true));
        coordinator->addComponent(enemy, SFML::Health(mapManager.getEnemyInfosById(id).life));
        id_to_entity.emplace(enemy_nbr, enemy);
        RType::Packet::CreateSpriteReference create_sprite(name, mapManager.getEnemyInfosById(id).texture_ref);
        auto packet = coordinator->getResource<Network::PackageManager>()->createPacket<RType::Packet::CreateSpriteReference>(create_sprite);
        coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
        RType::Packet::SpawnEntity entity_payload(enemy, name, 1000.0, 800.0);
        auto packetTwo = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::SpawnEntity>(entity_payload);
        coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packetTwo, sizeof(packetTwo), udp_handler);
        RType::Packet::TransformEntity entity_transform(enemy, coordinator->getComponent<SFML::Transform>(enemy));
        auto packetThree = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::TransformEntity>(entity_transform);
        coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packetThree, sizeof(packetThree), udp_handler);
        RType::Packet::SetEntityLinearMove linear_move(enemy, mapManager.getEnemyInfosById(id).speed, mapManager.getEnemyInfosById(id).dir, true);
        auto packetFour = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::SetEntityLinearMove>(linear_move);
        coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packetFour, sizeof(packetFour), udp_handler);

        return ++enemy_nbr;
    }

    /**
     * @brief Get the Entity from the EnemyID
     * @param id The EnemyID
     * @return The Entity associated with the EnemyID
     */
    ECS::Entity getEntityFromEnemyId(EnemyID id) { return id_to_entity[id]; }

private:
    std::unordered_map<EnemyID, ECS::Entity> id_to_entity;
    std::uint64_t enemy_nbr;
};
}