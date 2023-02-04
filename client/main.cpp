#include "../ecs/Coordinator.hpp"
#include "../common/PackageManager.hpp"
#include "../common/packet/PlayerName.hpp"
#include "../common/UDPHandler.hpp"
#include "../common/packet/GameStart.hpp"
#include "../sfml/Window.hpp"
#include "../sfml/Event.hpp"
#include "../sfml/TextureManager.hpp"
#include "../sfml/SpriteManager.hpp"
#include "Client.hpp"

std::pair<RType::Network::UDPClient, std::uint16_t> RType::Client::parseArguments(int ac, char **av) {
    if (ac != 4)
        throw std::invalid_argument("parseArguments: Wrong number of argument (3 arguments expected)");
    SFML::IpAddress address((std::string(av[1])));
    std::uint64_t port = std::stoul(av[2]);
    std::uint64_t client_port = std::stoul(av[3]);
    if (port > 65535 || client_port > 65535)
        throw std::invalid_argument("parseArguments: Port should be a number between 0 and 65535");
    return std::make_pair<RType::Network::UDPClient, std::uint16_t>(RType::Network::UDPClient(address, port), client_port);
}

void RType::Client::registerResources(std::unique_ptr<ECS::Coordinator> &coordinator, std::uint16_t port) {
    auto package_manager = coordinator->registerResource<RType::Network::PackageManager>();
    coordinator->registerResource<RType::Network::UDPHandler>(port, package_manager);
    coordinator->registerResource<SFML::EventManager>();
    coordinator->registerResource<SFML::TextureManager>();
    coordinator->registerResource<SFML::SpriteManager>();
}

void RType::Client::registerPackets(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();

    package_manager->registerPacket<RType::Packet::PlayerName>();
    package_manager->registerPacket<RType::Packet::GameStart>();
}

void RType::Client::loadAssets(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto texture_manager = coordinator->getResource<SFML::TextureManager>();
    auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();

    texture_manager->registerTexture("player_blue", "../assets/textures/player-blue.png");
    texture_manager->registerTexture("player_red", "../assets/textures/player-red.png");
    texture_manager->registerTexture("player_green", "../assets/textures/player-green.png");
    texture_manager->registerTexture("player_orange", "../assets/textures/player-orange.png");
    sprite_manager->registerSprite("player_blue", texture_manager->getTexture("player_blue"));
    sprite_manager->registerSprite("player_red", texture_manager->getTexture("player_red"));
    sprite_manager->registerSprite("player_green", texture_manager->getTexture("player_green"));
    sprite_manager->registerSprite("player_orange", texture_manager->getTexture("player_orange"));
}

void RType::Client::waiting_game_to_start(std::unique_ptr<ECS::Coordinator> &coordinator) {
    bool game_started = false;
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();

    std::cout << "Waiting for the game to start!" << std::endl;
    while (!game_started) {
        while(!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header || header->id != package_manager->getTypeId<RType::Packet::GameStart>())
                continue;
            game_started = true;
        }
    }
    std::cout << "Everyone joined! The game can finally start!" << std::endl;
}

void RType::Client::game_loop(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    auto event_manager = coordinator->getResource<SFML::EventManager>();
    auto window = coordinator->registerResource<SFML::Window>(1920, 1080, "Le R-Type", SFML::Window::Style::Default);
    SFML::Event event;

    std::cout << "Game loop started!" << std::endl;
    while (window->isOpen()) {
        while(window->pollEvent(event))
            event_manager->newEvent(event.getEvent());
        while(!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header)
                continue;
            std::cout << "Received packet with id: " << header->id << std::endl;
        }
        if (event_manager->quitEventRegistered())
            window->close();
        window->clear();
        window->display();
        event_manager->clear();
    }
}

/**
 * @brief main is the entry point of the RType Client
 * @param ac Number of command-line arguments
 * @param av Content of command-line arguments
 * @return 0 if the program exited correctly
 */
int main(int ac, char **av)
{
    auto [server_infos, client_port] = RType::Client::parseArguments(ac, av);
    auto coordinator = std::make_unique<ECS::Coordinator>();

    RType::Client::registerResources(coordinator, client_port);
    RType::Client::registerPackets(coordinator);

    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();

    RType::Packet::PlayerName player_name("AAAAAAAAAAA");
    auto packet = package_manager->createPacket<RType::Packet::PlayerName>(player_name);

    udp_handler->startHandler();
    udp_handler->send(&packet, sizeof(packet),std::string( "127.0.0.1"), 8080);
    RType::Client::loadAssets(coordinator);
    RType::Client::game_loop(coordinator);
    //RType::Client::waiting_game_to_start(coordinator);
    udp_handler->stopHandler();
    return (0);
}