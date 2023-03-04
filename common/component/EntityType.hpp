#pragma once

#include <string>

namespace SFML {
/**
 * @brief Enum used to define the entities type
 */
enum EntityTypeEnum { Player,
    Ennemie};
/**
 * @brief Component that set the entity type
 */
struct EntityType {
    /**
     * @brief Construct a new Entity Component
     *
     * @param type of entity 
     */
    EntityType(EntityTypeEnum type = EntityTypeEnum::Ennemie)
        : type(type) {};

    EntityTypeEnum type;
};
}