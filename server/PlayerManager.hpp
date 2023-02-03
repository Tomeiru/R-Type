#pragma once

#include <cstdint>
#include <unordered_map>
#include "../sfml/IpAddress.hpp"
#include "Types.hpp"
#include "../common/UDPClient.hpp"

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
        PlayerManager(std::uint16_t nb_player_max = 4) : nb_player_connected(0), id_to_client(), nb_player_max(nb_player_max) {}
        PlayerID addNewPlayer(const PlayerName &name, const Network::UDPClient &client) {
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
        std::uint16_t getNbPlayerConnected() const {
            return (nb_player_connected);
        }

        /**
         * @brief Sends a packet to all the connected player
         * @param data The packet to send
         * @param size The size of the packet
         * @param udp_handler The class that handles sending packet to other UDPClients
         */
        void sendPacketToAllPlayer(const void *data, size_t size, std::shared_ptr<Network::UDPHandler> &udp_handler) {
            for (const auto &[id, client]: id_to_client)
                udp_handler->send(data, size, client.getIpAddress(), client.getPort());
        }
    private:
        std::unordered_map<PlayerID, Network::UDPClient> id_to_client;
        std::unordered_map<PlayerID, PlayerName> id_to_name;
        std::unordered_map<PlayerName, PlayerID> name_to_id;
        std::uint16_t nb_player_connected;
        std::uint16_t nb_player_max;
    };
}