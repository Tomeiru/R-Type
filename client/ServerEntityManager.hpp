#pragma once

#include <unordered_map>
#include "../ecs/Types.hpp"

namespace RType::Client {
    /**
     * @brief ServerEntityManager is a class that manages the mapping between server entities and client entities
     */
    class ServerEntityManager {
    public:
        /**
         * @brief Register a server Entity corresponding to a Client Entity
         * @param server_entity Server Entity
         * @param client_entity Client Entity
         */
        void registerServerEntity(ECS::Entity server_entity, ECS::Entity client_entity)
        {
            server_entity_to_client_entity[server_entity] = client_entity;
            client_entity_to_server_entity[client_entity] = server_entity;
        }
        /**
         * @brief Get the Client Entity corresponding to a Server Entity
         * @param server_entity Server Entity
         * @return Client Entity
         */
         ECS::Entity getClientEntity(ECS::Entity server_entity)
         {
             return server_entity_to_client_entity[server_entity];
         }
        /**
         * @brief Get the Server Entity corresponding to a Client Entity
         * @param client_entity Client Entity
         * @return Server Entity
         */
         ECS::Entity getServerEntity(ECS::Entity client_entity)
         {
             return client_entity_to_server_entity[client_entity];
         }
    private:
        std::unordered_map<ECS::Entity, ECS::Entity> server_entity_to_client_entity;
        std::unordered_map<ECS::Entity, ECS::Entity> client_entity_to_server_entity;
    };
}
