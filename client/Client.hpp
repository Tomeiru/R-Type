#pragma once

#include "../common/UDPClient.hpp"
#include "../ecs/Coordinator.hpp"

namespace RType::Client {
/**
 * @brief waiting_game_to_start is a function that waits for a packet indicating that the game starts
 * @param coordinator Reference to the ECS' Coordinator
 */
void waiting_game_to_start(std::unique_ptr<ECS::Coordinator>& coordinator);

/**
 * @brief registerPackets is a function that registers all the packets used by the RType Client
 * @param coordinator Reference to the ECS' Coordinator
 */
void registerPackets(std::unique_ptr<ECS::Coordinator>& coordinator);

/**
 * @brief registerResources is a function that registers all the resources used by the RType Client
 * @param coordinator Reference to the ECS' Coordinator
 * @param port Port given in command-line argument
 */
void registerResources(std::unique_ptr<ECS::Coordinator>& coordinator, std::uint16_t port);

/**
 * @brief registerComponents is a function that registers all the components used by the RType Client
 * @param coordinator Reference to the ECS' Coordinator
 */
void registerComponents(std::unique_ptr<ECS::Coordinator>& coordinator);

/**
 * @brief registerSystems is a function that registers all the systems used by the RType Client
 * @param coordinator Reference to the ECS' Coordinator
 */
void registerSystems(std::unique_ptr<ECS::Coordinator>& coordinator);

/**
 * @brief loadAssets is a function that loads all the assets used by the RType Client
 * @param coordinator Reference to the ECS' Coordinator
 */
void loadAssets(std::unique_ptr<ECS::Coordinator>& coordinator);

/**
 * @brief sendMovementsKeys is a function that sends the movements keys to the server
 * @param coordinator Reference to the ECS' Coordinator
 * @param server Reference to the UDPClient containing the server's informations
 * @param key_checker Entity containing the InputKeys component
 */
void sendMovementsKeys(std::unique_ptr<ECS::Coordinator>& coordinator, const RType::Network::UDPClient& server, ECS::Entity key_checker);

/**
 * @brief parseArguments is a function that checks and returns parsed command-line arguments if they are correct
 * @param ac Number of command-line arguments
 * @param av Content of command-line arguments
 * @return The IP address and port given of the server and its own IP given in argument
 */
std::pair<RType::Network::UDPClient, std::uint16_t> parseArguments(int ac, char** av);

/**
 * @brief game_loop is a function that runs the game loop
 * @param coordinator Reference to the ECS' Coordinator
 * @param server_infos Reference to the UDPClient containing the server's informations
 */
void game_loop(std::unique_ptr<ECS::Coordinator>& coordinator, const RType::Network::UDPClient& server_infos);
}