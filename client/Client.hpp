#pragma once

#include "../ecs/Coordinator.hpp"
#include "../common/UDPClient.hpp"

namespace RType::Client {
    /**
     * @brief waiting_game_to_start is a function that waits for a packet indicating that the game starts
     * @param coordinator Reference to the ECS' Coordinator
     */
    void waiting_game_to_start(std::unique_ptr<ECS::Coordinator> &coordinator);

    /**
     * @brief registerPackets is a function that registers all the packets used by the RType Client
     * @param coordinator Reference to the ECS' Coordinator
     */
    void registerPackets(std::unique_ptr<ECS::Coordinator> &coordinator);

    /**
     * @brief registerResources is a function that registers all the resources used by the RType Client
     * @param coordinator Reference to the ECS' Coordinator
     * @param port Port given in command-line argument
     */
    void registerResources(std::unique_ptr<ECS::Coordinator> &coordinator, std::uint16_t port);

    /**
     * @brief parseArguments is a function that checks and returns parsed command-line arguments if they are correct
     * @param ac Number of command-line arguments
     * @param av Content of command-line arguments
     * @return The IP address and port given of the server and its own IP given in argument
     */
    std::pair<RType::Network::UDPClient, std::uint16_t>parseArguments(int ac, char **av);
}