#pragma once

#include "../common/PackageManager.hpp"
#include "../common/UDPHandler.hpp"
#include "../common/component/EntityType.hpp"
#include "../common/component/Hitbox.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/component/Transform.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../common/packet/TransformEntity.hpp"
#include "../ecs/Coordinator.hpp"
#include "../ecs/Types.hpp"
#include "PlayerManager.hpp"
#include "Types.hpp"
#include "component/Attack.hpp"
#include "component/BackupTransform.hpp"
#include "component/DestroyEntity.hpp"
#include "component/Direction.hpp"
#include "component/Speed.hpp"
#include <cstdint>
#include <memory>

namespace RType {
/**
 * @brief Class to manage bullets
 */
class BulletManager {
public:
    /**
     * @brief Construct a new BulletManager
     */
    BulletManager()
        : _bulletNumber(0) {};

    /**
     * @brief Get the number of bullets created
     * @return the number of bullets
     */
    std::uint64_t getBulletNumber() { return _bulletNumber; }

    /**
     * @brief Create a bullet with all the components
     * @param coordinator Reference to the ECS coordinator
     * @param attack Attack component contained in the entity that shoots
     * @param bulletTransform Transform to apply to the bullet
     */
    void createBullet(std::unique_ptr<ECS::Coordinator>& coordinator, SFML::Attack attack, const SFML::Transform& bulletTransform, SFML::EntityType type)
    {
        if (_bulletNumber == UINT64_MAX)
            return;
        auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
        auto bullet = coordinator->createEntity();
        auto texture_man = coordinator->getResource<SFML::TextureManager>();
        auto sprite_man = coordinator->getResource<SFML::SpriteManager>();
        std::string bulletId = "";
        if (attack.attackAngle == 180) {
            bulletId = "bullet_en_" + std::to_string(_bulletNumber);
        } else {
            bulletId = "bullet_pl_" + std::to_string(_bulletNumber);
        }
        coordinator->addComponent(bullet, SFML::Direction(attack.attackAngle));
        coordinator->addComponent(bullet, SFML::SpriteReference(bulletId));
        coordinator->addComponent(bullet, SFML::Transform(bulletTransform));
        coordinator->addComponent(bullet, SFML::BackupTransform(bulletTransform));
        coordinator->addComponent(bullet, SFML::Hitbox());
        float speed = setBulletSpeed(coordinator, bullet, attack.type);
        coordinator->addComponent(bullet, SFML::DestroyEntity(true, true, true));
        if (type.type == SFML::EntityTypeEnum::Player) {
            RType::Packet::CreateSpriteReference create_sprite(bulletId, "bulletTexturePlayer");
            sprite_man->registerSprite(bulletId, texture_man->getTexture("bulletTexturePlayer"));
            auto packet = coordinator->getResource<Network::PackageManager>()->createPacket<RType::Packet::CreateSpriteReference>(create_sprite);
            coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
        } else {
            RType::Packet::CreateSpriteReference create_sprite(bulletId, "bulletTextureEnemie");
            sprite_man->registerSprite(bulletId, texture_man->getTexture("bulletTextureEnemie"));
            auto packet = coordinator->getResource<Network::PackageManager>()->createPacket<RType::Packet::CreateSpriteReference>(create_sprite);
            coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
        }
        RType::Packet::SpawnEntity entity_spawn(bullet, bulletId, bulletTransform.position.getVector2().x, bulletTransform.position.getVector2().y, type);
        auto packetTwo = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::SpawnEntity>(entity_spawn);
        coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packetTwo, sizeof(packetTwo), udp_handler);
        RType::Packet::SetEntityLinearMove linear_move(bullet, speed * 10, attack.attackAngle, true);
        auto packetThree = coordinator->getResource<RType::Network::PackageManager>()->createPacket<RType::Packet::SetEntityLinearMove>(linear_move);
        coordinator->getResource<PlayerManager>()->sendPacketToAllPlayer(&packetThree, sizeof(packetThree), udp_handler);
        _id_to_entity.emplace(_bulletNumber, bullet);
        _bulletNumber++;
    }

    /**
     * @brief Get the ECS entity that content the bullet
     * @param id The bullet ID
     * @return The entity containing the bullet
     */
    ECS::Entity getEntityFromBulletId(BulletID id) { return _id_to_entity[id]; }

private:
    /**
     * @brief Function that sets the speed of a bullet
     *
     * @param coordinator Reference to the ecs coordinator
     * @param bullet Bullet entity to speed up
     * @param type Type of bullet to shoot
     * @return Speed of the bullet
     */
    float setBulletSpeed(std::unique_ptr<ECS::Coordinator>& coordinator, ECS::Entity bullet, SFML::AttackType type)
    {
        switch (type) {
        case SFML::AttackType::NormalAttack:
            coordinator->addComponent(bullet, SFML::Speed(2 * 10));
            return 2;
        case SFML::AttackType::FastAttack:
            coordinator->addComponent(bullet, SFML::Speed(4 * 10));
            return 4;
        case SFML::AttackType::VeryFastAttack:
            coordinator->addComponent(bullet, SFML::Speed(3 * 10));
            return 3;
        case SFML::AttackType::SlowAttack:
            coordinator->addComponent(bullet, SFML::Speed(1 * 10));
            return 1;
        default:
            coordinator->addComponent(bullet, SFML::Speed(2 * 10));
            return 2;
        }
    }

    std::uint64_t _bulletNumber;
    std::unordered_map<BulletID, ECS::Entity> _id_to_entity;
};
}