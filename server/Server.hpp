#pragma once

#include "../ecs/Coordinator.hpp"

namespace RType::Server {
    /**
    * @brief Function that waits for 4 players to join the game
    * @param coordinator Reference to the ECS' Coordinator
    */
    void waiting_for_players(std::unique_ptr<ECS::Coordinator> &coordinator);

    /**
     * @brief Function that loads all the assets used by the RType Server
     * @param coordinator Reference to the ECS' Coordinator
     */
    void loadAssets(std::unique_ptr<ECS::Coordinator> &coordinator);

    /**
    * @brief Function that registers all the packets used by the RType Server
    * @param coordinator Reference to the ECS' Coordinator
    */
    void registerPackets(std::unique_ptr<ECS::Coordinator> &coordinator);

    /**
    * @brief Function that registers all the resources used by the RType Server
    * @param coordinator Reference to the ECS' Coordinator
    * @param port Port given in command-line argument
    */
    void registerResources(std::unique_ptr<ECS::Coordinator> &coordinator, std::uint16_t port);

    /**
     * @brief Function that runs the game loop
     * @param coordinator Reference to the ECS' Coordinator
     */
    void game_loop(std::unique_ptr<ECS::Coordinator> &coordinator);

    /**
    * @brief Check and returns parsed command-line arguments if they are correct
    * @param ac Number of command-line arguments
    * @param av Content of command-line arguments
    * @return The port given in argument
    */
    std::uint16_t parseArguments(int ac, char **av);
}