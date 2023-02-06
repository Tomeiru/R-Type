#include <iostream>

#include "../ecs/RuntimeException.hpp"
#include "../common/PackageManager.hpp"
#include "../common/UDPHandler.hpp"
#include "../ecs/Coordinator.hpp"
#include "../server/PlayerManager.hpp"
#include "../common/packet/PlayerName.hpp"
#include "../common/packet/GameStart.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../sfml/TextureManager.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/component/Transform.hpp"
#include "Server.hpp"

std::uint16_t RType::Server::parseArguments(int ac, char **av) {
    if (ac != 2)
        throw std::invalid_argument("parseArguments: Wrong number of argument (2 arguments expected)");
    std::uint64_t port = std::stoul(av[1]);
    if (port > 65535)
        throw std::invalid_argument("parseArguments: Port should be a number between 0 and 65535");
    return (port);
}

void RType::Server::registerResources(std::unique_ptr<ECS::Coordinator> &coordinator, std::uint16_t port) {
    auto package_manager = coordinator->registerResource<RType::Network::PackageManager>();
    coordinator->registerResource<RType::Network::UDPHandler>(port, package_manager);
    coordinator->registerResource<RType::PlayerManager>();
    coordinator->registerResource<SFML::TextureManager>();
    coordinator->registerResource<SFML::SpriteManager>();
}

void RType::Server::registerComponents(std::unique_ptr<ECS::Coordinator> &coordinator) {
    coordinator->registerComponent<SFML::SpriteReference>();
    coordinator->registerComponent<SFML::Transform>();
}

void RType::Server::registerPackets(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();

    package_manager->registerPacket<RType::Packet::PlayerName>();
    package_manager->registerPacket<RType::Packet::GameStart>();
    package_manager->registerPacket<RType::Packet::SpawnEntity>();
}

void RType::Server::loadAssets(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto texture_manager = coordinator->getResource<SFML::TextureManager>();
    auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();

    texture_manager->registerTexture("player_blue", "../assets/textures/player-blue.png");
    texture_manager->registerTexture("player_red", "../assets/textures/player-red.png");
    texture_manager->registerTexture("player_green", "../assets/textures/player-green.png");
    texture_manager->registerTexture("player_orange", "../assets/textures/player-orange.png");
    sprite_manager->registerSprite("player_1", texture_manager->getTexture("player_blue"));
    sprite_manager->registerSprite("player_2", texture_manager->getTexture("player_red"));
    sprite_manager->registerSprite("player_3", texture_manager->getTexture("player_green"));
    sprite_manager->registerSprite("player_4", texture_manager->getTexture("player_orange"));
}

void RType::Server::waiting_for_players(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    auto player_manager = coordinator->getResource<RType::PlayerManager>();

    std::cout << "Waiting for 4 players to start the game!" << std::endl;
    while (player_manager->getNbPlayerConnected() < 4) {
        while(!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header || header->id != package_manager->getTypeId<RType::Packet::PlayerName>())
                continue;
            auto decoded_player_name = package_manager->decodeContent<RType::Packet::PlayerName>(packet_received.packet_data);
            player_manager->addNewPlayer(decoded_player_name->_name, packet_received.sender);
            std::cout << "A new player joined the game! We're now " << player_manager->getNbPlayerConnected() << std::endl;
        }
    }
    RType::Packet::GameStart game_start;
    auto packet = package_manager->createPacket<RType::Packet::GameStart>(game_start);
    std::cout << "Everyone joined! The game can finally start!" << std::endl;
    player_manager->sendPacketToAllPlayer(&packet, sizeof(packet), udp_handler);
}

void RType::Server::game_loop(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    auto player_manager = coordinator->getResource<RType::PlayerManager>();

    std::cout << "Game loop started!" << std::endl;
    player_manager->spawnPlayers(udp_handler, package_manager, coordinator);
    while (player_manager->getNbPlayerConnected() > 0) {
        while(!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header)
                continue;
        }
    }
}

/**
 * @brief Main function of the RType Server
 * @param ac Number of command-line arguments
 * @param av Content of command-line arguments
 * @return Status code (0 in case of success, 84 in case of failure)
 */
int main(int ac, char **av)
{
    try {
        std::uint16_t port = RType::Server::parseArguments(ac, av);
        auto coordinator = std::make_unique<ECS::Coordinator>();

        RType::Server::registerResources(coordinator, port);
        RType::Server::registerPackets(coordinator);
        RType::Server::registerComponents(coordinator);

        auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();

        udp_handler->startHandler();
        RType::Server::waiting_for_players(coordinator);
        RType::Server::loadAssets(coordinator);
        RType::Server::game_loop(coordinator);
        udp_handler->stopHandler();
    }
    catch(std::invalid_argument &e) {
        std::cerr << "An exception appeared in the argument checking part of the code: " << e.what() << std::endl;
        return (84);
    }
    catch (std::out_of_range &e) {
        std::cerr << "An exception appeared in the argument checking part of the code: " << "parseArguments: Port should be a number between 0 and 65535" << std::endl;
        return (84);
    }
    catch(ECS::RuntimeException &e) {
        std::cerr << "An exception appeared in the ECS part of the code: " << e.what() << std::endl;
        return (84);
    }
    /*catch (...) {
        std::cerr << "An unknown exception appeared" << std::endl;
        return (84);
    }*/

    return (0);
}