#include "../ecs/Coordinator.hpp"
#include "../common/PackageManager.hpp"
#include "../common/packet/PlayerName.hpp"
#include "../common/UDPHandler.hpp"
#include "../common/packet/GameStart.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../sfml/Window.hpp"
#include "../sfml/Event.hpp"
#include "../sfml/TextureManager.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/component/Transform.hpp"
#include "./component/Hitbox.hpp"
#include "./component/MovementKeys.hpp"
#include "./system/TransformSprite.hpp"
#include "./system/DrawSprite.hpp"
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

void RType::Client::registerComponents(std::unique_ptr<ECS::Coordinator> &coordinator) {
    coordinator->registerComponent<SFML::SpriteReference>();
    coordinator->registerComponent<SFML::Transform>();
    coordinator->registerComponent<SFML::Hitbox>();
    coordinator->registerComponent<SFML::MovementKeys>();
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
    package_manager->registerPacket<RType::Packet::SpawnEntity>();
}

void RType::Client::registerSystems(std::unique_ptr<ECS::Coordinator> &coordinator) {
    // coordinator->registerSystem<SFML::TransformSprite>();
    // coordinator->setSignatureBits<SFML::TransformSprite, SFML::SpriteReference, SFML::Transform>();
    // coordinator->registerSystem<SFML::DrawSprite>();
    // coordinator->setSignatureBits<SFML::DrawSprite, SFML::SpriteReference, SFML::Transform>();
}

void RType::Client::loadAssets(std::unique_ptr<ECS::Coordinator> &coordinator) {
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

void RType::Client::sendMovementsKeys(std::unique_ptr<ECS::Coordinator> &coordinator,
std::shared_ptr<RType::Network::UDPClient> &client, std::shared_ptr<RType::Network::PackageManager> &packageManager) {

}

void RType::Client::waiting_game_to_start(std::unique_ptr<ECS::Coordinator> &coordinator) {
    bool game_started = false;
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    std::queue<RType::Network::ReceivedPacket> tmp_queue;

    std::cout << "Waiting for the game to start!" << std::endl;
    while (!game_started) {
        while(!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header)
                continue;
            if (header->id == package_manager->getTypeId<RType::Packet::GameStart>()) {
                game_started = true;
                break;
            }
            tmp_queue.push(packet_received);
        }
        while (!tmp_queue.empty()) {
            udp_handler->pushQueue(tmp_queue.front());
            tmp_queue.pop();
        }
    }
    auto keyChecker = coordinator->createEntity();
    coordinator->addComponent<SFML::MovementKeys>(keyChecker, SFML::MovementKeys());
    std::cout << "Everyone joined! The game can finally start!" << std::endl;
}

void RType::Client::game_loop(std::unique_ptr<ECS::Coordinator> &coordinator) {
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    auto event_manager = coordinator->getResource<SFML::EventManager>();
    auto window = coordinator->registerResource<SFML::Window>(1920, 1080, "Le R-Type", SFML::Window::Style::Default);
    auto draw_sprite = coordinator->registerSystem<SFML::DrawSprite>();
    coordinator->setSignatureBits<SFML::DrawSprite, SFML::SpriteReference, SFML::Transform>();
    auto transform_sprite = coordinator->registerSystem<SFML::TransformSprite>();
    coordinator->setSignatureBits<SFML::TransformSprite, SFML::SpriteReference, SFML::Transform>();
    SFML::Event event;

    window->setFramerateLimit(60);
    while (window->isOpen()) {
        while(window->pollEvent(event))
            event_manager->newEvent(event.getEvent());
        while(!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header)
                continue;
            if (header->id == package_manager->getTypeId<RType::Packet::SpawnEntity>()) {
                auto packet = package_manager->decodeContent<RType::Packet::SpawnEntity>(packet_received.packet_data);
                auto player = coordinator->createEntity();
                coordinator->addComponent<SFML::SpriteReference>(player, SFML::SpriteReference(packet->_sprite_id));
                coordinator->addComponent<SFML::Transform>(player, SFML::Transform({packet->_x, packet->_y}, 0, {3, 3}));
            }
        }
        if (event_manager->quitEventRegistered())
            window->close();
        transform_sprite->update(coordinator);
        window->clear();
        draw_sprite->update(coordinator);
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
    RType::Client::registerComponents(coordinator);

    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();

    RType::Packet::PlayerName player_name("AAAAAAAAAAA");
    auto packet = package_manager->createPacket<RType::Packet::PlayerName>(player_name);

    udp_handler->startHandler();
    udp_handler->send(&packet, sizeof(packet),std::string( "127.0.0.1"), server_infos.getPort());
    RType::Client::loadAssets(coordinator);
    RType::Client::waiting_game_to_start(coordinator);
    RType::Client::game_loop(coordinator);
    udp_handler->stopHandler();
    return (0);
}