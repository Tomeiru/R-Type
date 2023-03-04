#pragma once

#include <string>
#include "../component/EntityType.hpp"

namespace RType::Packet {
/**
 * @brief SpawnEntity is a class that represents a spawn entity packet
 */

struct SpawnEntity {
    /**
     * @brief Constructs a new SpawnEntity object
     * @param id The id of the entity
     * @param type The type of the entity
     * @param x The x position of the entity
     * @param y The y position of the entity
     */
    SpawnEntity(ECS::Entity entity = 0, std::string sprite_id = "", float x = 0, float y = 0, SFML::EntityType type = SFML::EntityTypeEnum::Ennemie)
        : _entity(entity)
        , _x(x)
        , _y(y)
        , _type(type)
    {
        for (int i = 0; i < 15; i++) {
            _sprite_id[i] = (i < sprite_id.size() ? sprite_id[i] : '\0');
        }
        _sprite_id[15] = '\0';
    };

    ECS::Entity _entity;
    char _sprite_id[16];
    float _x;
    float _y;
    SFML::EntityType _type;
};
};