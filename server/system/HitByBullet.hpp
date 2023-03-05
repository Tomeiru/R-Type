#pragma once

#include "../../common/component/Hitbox.hpp"
#include "../../common/component/SpriteReference.hpp"
#include "../../common/component/Transform.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../sfml/SpriteManager.hpp"
#include "../PlayerManager.hpp"
#include "../component/DestroyEntity.hpp"
#include "../component/Health.hpp"

namespace SFML {
/**
 * @brief System that detect's collisions between bullets and player/enemy's
 */
class HitByBullet : public ECS::System {

public:
    /**
     * @brief Function that checks collisions between enetities
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator, int32_t elapsed)
    {
        lastTm += elapsed;
        // if (lastTm < 500)
        // return;
        lastTm = 0;
        auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();
        std::queue<ECS::Entity> toKill;
        for (const auto& entity : entities) {
            auto& sprite_ref = coordinator->getComponent<SpriteReference>(entity);
            auto& transform = coordinator->getComponent<Transform>(entity);
            auto sprite = sprite_manager->getSprite(sprite_ref.id);

            auto& hitbox = coordinator->getComponent<Hitbox>(entity);
            sprite->setPosition(transform.position);
            sprite->setRotation(transform.rotation);
            sprite->setScale(transform.scale);
            sprite->setOrigin(transform.origin);
            hitbox.rect = sprite->getGlobalBounds();

            for (const auto& entity2 : entities) {
                if (entity2 == entity)
                    continue;
                auto& sprite_ref2 = coordinator->getComponent<SpriteReference>(entity2);
                auto& transform2 = coordinator->getComponent<Transform>(entity2);
                auto sprite2 = sprite_manager->getSprite(sprite_ref2.id);

                auto& hitbox2 = coordinator->getComponent<Hitbox>(entity2);
                sprite2->setPosition(transform2.position);
                sprite2->setRotation(transform2.rotation);
                sprite2->setScale(transform2.scale);
                sprite2->setOrigin(transform2.origin);
                hitbox2.rect = sprite2->getGlobalBounds();
                if (hitbox2.rect.intersects(hitbox.rect)) {
                    if (coordinator->hasComponent<Health>(entity2) && !coordinator->hasComponent<Health>(entity)) {
                        if ((sprite_ref2.id.rfind("enemy") != std::string::npos && sprite_ref.id.find("bullet_enemy") == std::string::npos) || (sprite_ref2.id.rfind("player") != std::string::npos && sprite_ref.id.find("bullet_player") == std::string::npos)) {
                            std::cout << sprite_ref2.id << " " << sprite_ref.id << std::endl;
                            auto& health2 = coordinator->getComponent<Health>(entity2);

                            if (health2.healthPoints != 0)
                                health2.healthPoints -= 1;
                            std::cout << sprite_ref2.id << "(" << health2.healthPoints << ")"
                                      << " " << sprite_ref.id << std::endl;
                            toKill.push(entity);
                        }
                    }
                }
            }
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
    };

private:
    int32_t lastTm = 0;
};
}