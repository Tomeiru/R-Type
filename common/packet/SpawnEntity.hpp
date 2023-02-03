#pragma once

#include <string>

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
        SpawnEntity(std::uint32_t id = 0, std::uint32_t textureId = 0,float x = 0, float y = 0) :id(id), x(x), y(y), textureId(textureId) {};

        std::uint32_t id;
        std::uint32_t textureId;
        float x;
        float y;
    };
};