#pragma once

#include "../common/UDPClient.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/component/Transform.hpp"
#include "../common/packet/GameStart.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../common/component/EntityType.hpp"
#include "../sfml/IpAddress.hpp"
#include "Types.hpp"
#include "component/Attack.hpp"
#include <cstdint>
#include <unordered_map>

namespace RType {
/**
 * @brief PlayerManager is the container that handles player registration
 */
class PlayerManager {
public:
    /**
     * @brief Constructs a new PlayerManager object
     * @param nb_player_max Maximal number of player the server accepts
     */
    PlayerManager(std::uint16_t nb_player_max = 4)
        : nb_player_connected(0)
        , id_to_client()
        , nb_player_max(nb_player_max)
    {
    }

    /**
     * @brief Add a new player the PlayerManager
     * @param name Name to give to the player
     * @param client Information about the client containing the ip adress etc
     * @return ID of the player
     */
    PlayerID addNewPlayer(const PlayerName& name,
        const Network::UDPClient& client)
    {
        if (nb_player_connected == nb_player_max)
            return (0);
        nb_player_connected += 1;
        id_to_client.emplace(nb_player_connected, client);
        id_to_name.emplace(nb_player_connected, name);
        name_to_id.emplace(name, nb_player_connected);
        return (nb_player_connected);
    }

    /**
     * @brief Get the current number of connected player
     * @return The current number of connected player
     */
    std::uint16_t getNbPlayerConnected() const { return (nb_player_connected); }

    /**
     * @brief Sends a packet to all the connected player
     * @param data The packet to send
     * @param size The size of the packet
     * @param udp_handler The class that handles sending packet to other
     * UDPClients
     */
    void
    sendPacketToAllPlayer(const void* data, size_t size,
        std::shared_ptr<Network::UDPHandler>& udp_handler)
    {
        for (const auto& [id, client] : id_to_client)
            udp_handler->send(data, size, client.getIpAddress(), client.getPort());
    }

    /**
     * @brief Sends a GameStart packet to all the connected player
     * @param udp_handler The class that handles sending packet to other
     * UDPClients
     * @param package_manager Package manager, used to create and send packets
     */
    void sendGameStartToAllPlayers(
        std::shared_ptr<Network::UDPHandler>& udp_handler,
        std::shared_ptr<RType::Network::PackageManager>& package_manager)
    {
        for (const auto& [id, client] : id_to_client) {
            RType::Packet::GameStart game_start(id);
            auto packet = package_manager->createPacket<RType::Packet::GameStart>(
                game_start, id);
            udp_handler->send(&packet, sizeof(packet), client.getIpAddress(),
                client.getPort());
        }
    }

    /**
     * @brief Spawn a new player in the game
     * @param udp_handler Network manager using UDP to communicate
     * @param package_manager Package manager, used to create and send packets
     * @param coordinator Reference to the ecs coordinator
     */
    void
    spawnPlayers(std::shared_ptr<Network::UDPHandler>& udp_handler,
        std::shared_ptr<RType::Network::PackageManager>& package_manager,
        std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        for (const auto& [id, client] : id_to_client) {
            auto player = coordinator->createEntity();
            id_to_entity.emplace(id, player);
            std::string name = "player_" + std::to_string(id);
            coordinator->addComponent(player, SFML::SpriteReference(name));
            coordinator->addComponent(
                player,
                SFML::Transform({ 0, static_cast<float>(200 * id) }, 0, { 3, 3 }));
            coordinator->addComponent(player, SFML::Attack(false, 200, SFML::AttackType::NormalAttack, 0));
            coordinator->addComponent(player, SFML::EntityType(SFML::EntityTypeEnum::Player));

            RType::Packet::SpawnEntity entity_payload(player, name, 0,
                static_cast<float>(200 * id), SFML::EntityTypeEnum::Player);
            auto packet = package_manager->createPacket<RType::Packet::SpawnEntity>(
                entity_payload);
            sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
        }
        std::cout << "All players spawned" << std::endl;
    }

    /**
     * @brief Get the Entity from a PlayerID
     * @param id The PlayerID
     * @return The Entity associated with the PlayerID
     */
    ECS::Entity getEntityFromPlayerID(PlayerID id) { return (id_to_entity[id]); }

private:
    std::unordered_map<PlayerID, Network::UDPClient> id_to_client;
    std::unordered_map<PlayerID, PlayerName> id_to_name;
    std::unordered_map<PlayerName, PlayerID> name_to_id;
    std::unordered_map<PlayerID, ECS::Entity> id_to_entity;
    std::uint16_t nb_player_connected;
    std::uint16_t nb_player_max;
};
} // namespace RType
