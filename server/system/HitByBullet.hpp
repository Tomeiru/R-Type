#pragma once

#include "../../common/component/Hitbox.hpp"
#include "../../ecs/Coordinator.hpp"
#include "../../common/component/SpriteReference.hpp"
#include "../../common/component/Transform.hpp"
#include "../../sfml/SpriteManager.hpp"
#include "../component/Health.hpp"
#include "../component/DestroyEntity.hpp"
#include "../PlayerManager.hpp"


namespace SFML {
/**
 * @brief System that detect's collisions between bullets and player/enemy's
 */
class HitByBullet: public ECS::System {

    public:
    /**
     * @brief Function that checks collisions between enetities
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator, int32_t elapsed)
    {
        lastTm += elapsed;
        if (lastTm < 500)
            return;
        lastTm = 0;
        auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();
        std::queue<ECS::Entity> toKill;
        for (const auto& entity : entities) {
            auto& sprite_ref = coordinator->getComponent<SpriteReference>(entity);
            auto& transform = coordinator->getComponent<Transform>(entity);
            auto sprite = sprite_manager->getSprite(sprite_ref.id);

            auto &hitbox = coordinator->getComponent<Hitbox>(entity);
            hitbox.rect = sprite->getGlobalBounds();
            hitbox.rect.setTopLeft(transform.position.getX(), transform.position.getY());

            for (const auto &entity2 : entities) {
                if (entity2 == entity)
                    continue;
                auto& sprite_ref2 = coordinator->getComponent<SpriteReference>(entity2);
                auto& transform2 = coordinator->getComponent<Transform>(entity2);
                auto sprite2 = sprite_manager->getSprite(sprite_ref2.id);

                auto &hitbox2 = coordinator->getComponent<Hitbox>(entity2);
                hitbox2.rect = sprite2->getGlobalBounds();
                hitbox2.rect.setTopLeft(transform2.position.getX(), transform2.position.getY());
                std::cout << sprite_ref2.id << " : " << hitbox2.rect._rect.left << "-" << hitbox2.rect._rect.width << " " << hitbox2.rect._rect.top << "-" << hitbox2.rect._rect.height << std::endl;
                std::cout << sprite_ref.id << " : " << hitbox.rect._rect.left << "-" << hitbox.rect._rect.width << " " << hitbox.rect._rect.top << "-" << hitbox.rect._rect.height << std::endl;
                if (hitbox2.rect.intersects(hitbox.rect)) {
                    if (coordinator->hasComponent<Health>(entity2) && !coordinator->hasComponent<Health>(entity)) {
                            auto &health2 = coordinator->getComponent<Health>(entity2);

                        std::cout << sprite_ref2.id << "(" << health2.healthPoints << ")" << " " << sprite_ref.id << std::endl;
                        health2.healthPoints -= 1;
                        toKill.push(entity);
                    }
                }
                /*if (coordinator->hasComponent<Health>(entity2)) {
                    if (!coordinator->hasComponent<Health>(entity)) {
                        auto &health2 = coordinator->getComponent<Health>(entity2);

                        health2.healthPoints -= 1;
                        auto& destroyStat = coordinator->getComponent<SFML::DestroyEntity>(entity);

                        toDestroy.push(std::pair(entity, destroyStat.sprite));
                    }
                } else if (coordinator->hasComponent<Health>(entity)) {
                    auto &health = coordinator->getComponent<Health>(entity);

                    health.healthPoints -= 1;
                }*/
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