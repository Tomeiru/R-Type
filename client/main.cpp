#include "../common/PackageManager.hpp"
#include "../common/UDPHandler.hpp"
#include "../common/component/Hitbox.hpp"
#include "../common/component/SpriteReference.hpp"
#include "../common/component/Tint.hpp"
#include "../common/component/Transform.hpp"
#include "../common/packet/CreateSpriteReference.hpp"
#include "../common/packet/DestroyEntity.hpp"
#include "../common/packet/EntityPosition.hpp"
#include "../common/packet/GameStart.hpp"
#include "../common/packet/PlayerInputs.hpp"
#include "../common/packet/PlayerName.hpp"
#include "../common/packet/SpawnEntity.hpp"
#include "../common/packet/TransformEntity.hpp"
#include "../common/system/LinearMove.hpp"
#include "../sfml/ColorManager.hpp"
#include "../sfml/Event.hpp"
#include "../sfml/FontManager.hpp"
#include "../sfml/MusicManager.hpp"
#include "../sfml/SoundBufferManager.hpp"
#include "../sfml/SoundManager.hpp"
#include "../sfml/SpriteManager.hpp"
#include "../sfml/TextManager.hpp"
#include "../sfml/TextureManager.hpp"
#include "../sfml/Window.hpp"
#include "./component/Clickable.hpp"
#include "./component/Hover.hpp"
#include "./component/HoverTint.hpp"
#include "./component/InputKeys.hpp"
#include "./component/MusicReference.hpp"
#include "./component/SoundReference.hpp"
#include "./component/TextReference.hpp"
#include "./system/DrawSprite.hpp"
#include "./system/DrawText.hpp"
#include "./system/PlayMusic.hpp"
#include "./system/PlaySound.hpp"
#include "./system/TintText.hpp"
#include "./system/TransformSprite.hpp"
#include "./system/TransformText.hpp"
#include "./system/UpdateClick.hpp"
#include "./system/UpdateHover.hpp"
#include "./system/UpdateHoverTint.hpp"
#include "./system/UpdateKeysInput.hpp"
#include "Client.hpp"
#include "PacketManager.hpp"
#include "PlayerID.hpp"
#include "ServerEntityManager.hpp"

/**
 * @brief Function that parse arguments
 *
 * @param ac Number of argument
 * @param av arguments
 * @return Pair containing the IP address and port of the server and the client port
 */
std::pair<RType::Network::UDPClient, std::uint16_t> RType::Client::parseArguments(int ac, char** av)
{
    if (ac != 4)
        throw std::invalid_argument("parseArguments: Wrong number of argument (3 arguments expected)");
    SFML::IpAddress address((std::string(av[1])));
    std::uint64_t port = std::stoul(av[2]);
    std::uint64_t client_port = std::stoul(av[3]);
    if (port > 65535 || client_port > 65535)
        throw std::invalid_argument("parseArguments: Port should be a number between 0 and 65535");
    return std::make_pair<RType::Network::UDPClient, std::uint16_t>(RType::Network::UDPClient(address, port), client_port);
}

/**
 * @brief Function which is used as callback for the quit button
 *
 * @param coordinator Reference to the ecs coordinator
 * @param entity Entity of the button
 */
void quitButtonCallback(std::unique_ptr<ECS::Coordinator>& coordinator, ECS::Entity entity)
{
    auto window = coordinator->getResource<SFML::Window>();
    window->close();
}

/**
 * @brief Function which is used as callback for the play button
 *
 * @param coordinator Reference to the ecs coordinator
 * @param entity Entity of the button
 */
void playButtonCallback(std::unique_ptr<ECS::Coordinator>& coordinator, ECS::Entity entity)
{
    auto scene_manager = coordinator->getResource<RType::Client::SceneManager>();
    scene_manager->setCurrentScene(RType::Client::SceneManager::Scene::LOBBY);
}

/**
 * @brief Function that register every component used in the client to the ecs
 *
 * @param coordinator Reference to the ecs coordinator
 */
void RType::Client::registerComponents(std::unique_ptr<ECS::Coordinator>& coordinator)
{
    coordinator->registerComponent<SFML::SpriteReference>();
    coordinator->registerComponent<SFML::Transform>();
    coordinator->registerComponent<SFML::Hitbox>();
    coordinator->registerComponent<SFML::InputKeys>();
    coordinator->registerComponent<SFML::Direction>();
    coordinator->registerComponent<SFML::Speed>();
    coordinator->registerComponent<SFML::TextReference>();
    coordinator->registerComponent<SFML::HoverTint>();
    coordinator->registerComponent<SFML::Hover>();
    coordinator->registerComponent<SFML::Tint>();
    coordinator->registerComponent<SFML::Clickable>();
    coordinator->registerComponent<SFML::MusicReference>();
    coordinator->registerComponent<SFML::SoundReference>();
}

/**
 * @brief Function that register every resource used in the client to the ecs
 *
 * @param coordinator Reference to the ecs coordinator
 * @param port Port of the client, used to create the UDPHandler
 */
void RType::Client::registerResources(std::unique_ptr<ECS::Coordinator>& coordinator, std::uint16_t port)
{
    auto package_manager = coordinator->registerResource<RType::Network::PackageManager>();
    coordinator->registerResource<SFML::Window>(1920, 1080, "Le R-Type", SFML::Window::Style::Default);
    coordinator->registerResource<RType::Network::UDPHandler>(port, package_manager);
    coordinator->registerResource<SFML::EventManager>();
    coordinator->registerResource<SFML::TextureManager>();
    coordinator->registerResource<SFML::SpriteManager>();
    coordinator->registerResource<SFML::Clock>();
    coordinator->registerResource<SFML::FontManager>();
    coordinator->registerResource<SFML::TextManager>();
    coordinator->registerResource<RType::Client::SceneManager>();
    coordinator->registerResource<SFML::ColorManager>();
}

/**
 * @brief Function that register every packet used in the client to the ecs
 *
 * @param coordinator Reference to the ecs coordinator
 */
void RType::Client::registerPackets(std::unique_ptr<ECS::Coordinator>& coordinator)
{
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();

    package_manager->registerPacket<RType::Packet::PlayerName>();
    package_manager->registerPacket<RType::Packet::GameStart>();
    package_manager->registerPacket<RType::Packet::SpawnEntity>();
    package_manager->registerPacket<RType::Packet::PlayerInputs>();
    package_manager->registerPacket<RType::Packet::EntityPosition>();
    package_manager->registerPacket<RType::Packet::TransformEntity>();
    package_manager->registerPacket<RType::Packet::DestroyEntity>();
    package_manager->registerPacket<RType::Packet::CreateSpriteReference>();
    package_manager->registerPacket<RType::Packet::SetEntityLinearMove>();
}

/**
 * @brief Function that register every systems used in the client to the ecs
 *
 * @param coordinator Reference to the ecs coordinator
 */
void RType::Client::registerSystems(std::unique_ptr<ECS::Coordinator>& coordinator)
{
    coordinator->registerSystem<SFML::TransformSprite>();
    coordinator->setSignatureBits<SFML::TransformSprite, SFML::SpriteReference, SFML::Transform>();
    coordinator->registerSystem<SFML::DrawSprite>();
    coordinator->setSignatureBits<SFML::DrawSprite, SFML::SpriteReference, SFML::Transform>();
    coordinator->registerSystem<SFML::UpdateKeysInput>();
    coordinator->setSignatureBits<SFML::UpdateKeysInput, SFML::InputKeys>();
    coordinator->registerSystem<SFML::LinearMove>();
    coordinator->setSignatureBits<SFML::LinearMove, SFML::Speed, SFML::Direction, SFML::Transform>();
    coordinator->registerSystem<SFML::DrawText>();
    coordinator->setSignatureBits<SFML::DrawText, SFML::TextReference, SFML::Transform>();
    coordinator->registerSystem<SFML::TransformText>();
    coordinator->setSignatureBits<SFML::TransformText, SFML::TextReference, SFML::Transform>();
    coordinator->registerSystem<SFML::UpdateHoverTint>();
    coordinator->setSignatureBits<SFML::UpdateHoverTint, SFML::HoverTint, SFML::Hover, SFML::Tint>();
    coordinator->registerSystem<SFML::TintText>();
    coordinator->setSignatureBits<SFML::TintText, SFML::TextReference, SFML::Tint>();
    coordinator->registerSystem<SFML::UpdateHover>();
    coordinator->setSignatureBits<SFML::UpdateHover, SFML::Hover, SFML::Hitbox>();
    coordinator->registerSystem<SFML::UpdateClick>();
    coordinator->setSignatureBits<SFML::UpdateClick, SFML::Hitbox, SFML::Clickable>();
    coordinator->registerSystem<SFML::PlayMusic>();
    coordinator->setSignatureBits<SFML::PlayMusic, SFML::MusicReference>();
    coordinator->registerSystem<SFML::PlaySound>();
    coordinator->setSignatureBits<SFML::PlaySound, SFML::SoundReference>();
}

/**
 * @brief Function that loads the assets (textures, sprites, texts, ...)
 *
 * @param coordinator Reference to the ecs coordinator
 */
void RType::Client::loadAssets(std::unique_ptr<ECS::Coordinator>& coordinator)
{
    auto texture_manager = coordinator->getResource<SFML::TextureManager>();
    auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();
    auto font_manager = coordinator->getResource<SFML::FontManager>();
    auto text_manager = coordinator->getResource<SFML::TextManager>();
    auto color_manager = coordinator->getResource<SFML::ColorManager>();
    auto backgroundMusic = coordinator->registerResource<SFML::MusicManager>();
    auto player_bullet_buffer = coordinator->registerResource<SFML::SoundBufferManager>();
    auto player_bullet = coordinator->registerResource<SFML::SoundManager>();

    texture_manager->registerTexture("player_blue", "../assets/textures/player-blue.png");
    texture_manager->registerTexture("player_red", "../assets/textures/player-red.png");
    texture_manager->registerTexture("player_green", "../assets/textures/player-green.png");
    texture_manager->registerTexture("player_orange", "../assets/textures/player-orange.png");
    texture_manager->registerTexture("logo", "../assets/textures/logo.png");
    texture_manager->registerTexture("enemyTexture", "../assets/textures/player-red.png");
    texture_manager->registerTexture("blue", "../assets/textures/sky_lightened.png");
    texture_manager->registerTexture("clouds_bg", "../assets/textures/clouds_bg.png");
    texture_manager->registerTexture("mountains", "../assets/textures/glacial_mountains.png");
    texture_manager->registerTexture("clouds", "../assets/textures/clouds_mg_2.png");
    texture_manager->registerTexture("bulletTexturePlayer", "../assets/textures/bulletPlayer.png");
    texture_manager->registerTexture("bulletTextureEnemie", "../assets/textures/bulletEnnemie.png");
    texture_manager->registerTexture("enemy_A", "../assets/textures/enemy.png");
    texture_manager->registerTexture("enemy_B", "../assets/textures/enemy2.png");
    sprite_manager->registerSprite("player_1", texture_manager->getTexture("player_blue"));
    sprite_manager->registerSprite("player_2", texture_manager->getTexture("player_red"));
    sprite_manager->registerSprite("player_3", texture_manager->getTexture("player_green"));
    sprite_manager->registerSprite("player_4", texture_manager->getTexture("player_orange"));
    sprite_manager->registerSprite("background", texture_manager->getTexture("blue"));
    sprite_manager->registerSprite("parallax3", texture_manager->getTexture("clouds_bg"));
    sprite_manager->registerSprite("parallax2", texture_manager->getTexture("mountains"));
    sprite_manager->registerSprite("parallax1", texture_manager->getTexture("clouds"));
    sprite_manager->registerSprite("logo", texture_manager->getTexture("logo"));
    font_manager->registerFont("r_type", "../assets/fonts/r-type.ttf");
    text_manager->registerText("quit_button", "QUIT", font_manager->getFont("r_type"), 50);
    text_manager->registerText("play_button", "PLAY", font_manager->getFont("r_type"), 50);
    text_manager->registerText("waiting_text", "Waiting for other players...", font_manager->getFont("r_type"), 50);
    color_manager->registerRGBColor("purple", 255, 0, 255);
    color_manager->registerHexColor("white", 0xffffffff);
    backgroundMusic->registerMusic("background_music", "../assets/audio/rtype.ogg", true);
    player_bullet_buffer->registerSoundBuffer("player_bullet", "../assets/audio/player_bullet.ogg");
    player_bullet->registerSound("player_bullet", player_bullet_buffer->getSoundBuffer("player_bullet"), false);
}

/**
 * @brief Function that sends a packet of the client inputs to the server
 *
 * @param coordinator Reference to the ecs coordinator
 * @param server IP address and port of the server
 * @param key_checker Entity that corresponds to the key checker (multiple is useless)
 */
void RType::Client::sendMovementsKeys(std::unique_ptr<ECS::Coordinator>& coordinator,
    const RType::Network::UDPClient& server, ECS::Entity key_checker)
{
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto player_id = coordinator->getResource<RType::Client::PlayerID>();
    const auto& keys = coordinator->getComponent<SFML::InputKeys>(key_checker);

    if (keys.key_left_pressed == false && keys.key_right_pressed == false && keys.key_up_pressed == false && keys.key_down_pressed == false && keys.key_shoot_pressed == false)
        return;
    RType::Packet::PlayerInputs inputs(keys.key_left_pressed, keys.key_right_pressed, keys.key_up_pressed, keys.key_down_pressed, keys.key_shoot_pressed);
    auto packet = package_manager->createPacket(inputs, player_id->id);
    udp_handler->send(&packet, sizeof(packet), server.getIpAddress(), server.getPort());
}

/**
 * @brief Display and manage the menu
 *
 * @param coordinator Reference to the ecs coordinator
 * @return Scene to change to
 */
RType::Client::SceneManager::Scene RType::Client::display_menu(std::unique_ptr<ECS::Coordinator>& coordinator)
{
    auto event_manager = coordinator->getResource<SFML::EventManager>();
    auto sprite_manager = coordinator->getResource<SFML::SpriteManager>();
    auto scene_manager = coordinator->getResource<RType::Client::SceneManager>();
    auto window = coordinator->getResource<SFML::Window>();
    auto draw_sprite = coordinator->getSystem<SFML::DrawSprite>();
    auto draw_text = coordinator->getSystem<SFML::DrawText>();
    auto transform_sprite = coordinator->getSystem<SFML::TransformSprite>();
    auto transform_text = coordinator->getSystem<SFML::TransformText>();
    auto update_hover_tint = coordinator->getSystem<SFML::UpdateHoverTint>();
    auto update_click = coordinator->getSystem<SFML::UpdateClick>();
    auto tint_text = coordinator->getSystem<SFML::TintText>();
    auto update_hover = coordinator->getSystem<SFML::UpdateHover>();
    auto play_music = coordinator->getSystem<SFML::PlayMusic>();
    auto logo = coordinator->createEntity();
    auto quit_button = coordinator->createEntity();
    auto play_button = coordinator->createEntity();
    auto background_music = coordinator->createEntity();
    SFML::Event event;

    coordinator->addComponent(background_music, SFML::MusicReference("background_music", SFML::Music::Playing));
    coordinator->addComponent<SFML::SpriteReference>(logo, SFML::SpriteReference("logo"));
    coordinator->addComponent<SFML::Transform>(logo, SFML::Transform({ 600, 100 }, 0, { 1.5, 1.5 }));
    coordinator->addComponent<SFML::TextReference>(quit_button, SFML::TextReference("quit_button"));
    coordinator->addComponent<SFML::Transform>(quit_button, SFML::Transform({ 850, 700 }, 0, { 1, 1 }));
    coordinator->addComponent(quit_button, SFML::Hitbox());
    coordinator->addComponent(quit_button, SFML::Hover());
    coordinator->addComponent(quit_button, SFML::Tint("white"));
    coordinator->addComponent(quit_button, SFML::HoverTint("purple", "white"));
    coordinator->addComponent<SFML::Clickable>(quit_button, SFML::Clickable(quitButtonCallback));
    coordinator->addComponent<SFML::TextReference>(play_button, SFML::TextReference("play_button"));
    coordinator->addComponent<SFML::Transform>(play_button, SFML::Transform({ 850, 600 }, 0, { 1, 1 }));
    coordinator->addComponent(play_button, SFML::Hitbox());
    coordinator->addComponent(play_button, SFML::Hover());
    coordinator->addComponent(play_button, SFML::Tint("white"));
    coordinator->addComponent(play_button, SFML::HoverTint("purple", "white"));
    coordinator->addComponent<SFML::Clickable>(play_button, SFML::Clickable(playButtonCallback));
    window->setFramerateLimit(60);
    while (window->isOpen() && scene_manager->getCurrentScene() == RType::Client::SceneManager::Scene::MAIN_MENU) {
        while (window->pollEvent(event))
            event_manager->newEvent(event.getEvent());
        if (event_manager->quitEventRegistered())
            window->close();
        std::cout << "Scene: " << scene_manager->getCurrentScene() << std::endl;
        transform_sprite->update(coordinator);
        transform_text->update(coordinator);
        play_music->update(coordinator);
        update_hover->update(coordinator);
        update_hover_tint->update(coordinator);
        update_click->update(coordinator);
        tint_text->update(coordinator);
        window->clear();
        draw_text->update(coordinator);
        draw_sprite->update(coordinator);
        window->display();
        event_manager->clear();
    }
    coordinator->destroyEntity(logo);
    coordinator->destroyEntity(quit_button);
    coordinator->destroyEntity(play_button);
    if (!window->isOpen())
        return RType::Client::SceneManager::Scene::QUIT;
    return RType::Client::SceneManager::Scene::LOBBY;
}

/**
 * @brief Player connection system, waiting for other players to connect to the game
 *
 * @param coordinator Reference to the ecs coordinator
 */
void RType::Client::waiting_game_to_start(std::unique_ptr<ECS::Coordinator>& coordinator)
{
    bool game_started = false;
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    auto window = coordinator->getResource<SFML::Window>();
    auto transform_text = coordinator->getSystem<SFML::TransformText>();
    auto draw_text = coordinator->getSystem<SFML::DrawText>();
    auto event_manager = coordinator->getResource<SFML::EventManager>();
    auto waiting_message = coordinator->createEntity();
    std::queue<RType::Network::ReceivedPacket> tmp_queue;
    SFML::Event event;

    std::cout << "Waiting for the game to start!" << std::endl;
    coordinator->addComponent<SFML::TextReference>(waiting_message, SFML::TextReference("waiting_text"));
    coordinator->addComponent<SFML::Transform>(waiting_message, SFML::Transform({ 300, 400 }, 0, { 1, 1 }));
    while (!game_started && window->isOpen()) {
        while (window->pollEvent(event))
            event_manager->newEvent(event.getEvent());
        while (!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header)
                continue;
            if (header->id == package_manager->getTypeId<RType::Packet::GameStart>()) {
                auto decoded_game_start = package_manager->decodeContent<RType::Packet::GameStart>(packet_received.packet_data);
                coordinator->registerResource<RType::Client::PlayerID>(decoded_game_start->player_id);
                game_started = true;
                break;
            }
            tmp_queue.push(packet_received);
        }
        while (!tmp_queue.empty()) {
            udp_handler->pushQueue(tmp_queue.front());
            tmp_queue.pop();
        }
        if (event_manager->quitEventRegistered())
            window->close();
        transform_text->update(coordinator);
        window->clear();
        draw_text->update(coordinator);
        window->display();
        event_manager->clear();
    }
    std::cout << "Everyone joined! The game can finally start!" << std::endl;
}

/**
 * @brief Main loop of the game, once the game starts
 *
 * @param coordinator Reference to the ecs coordinator
 * @param server_infos IP address and port of the server
 */
void RType::Client::game_loop(std::unique_ptr<ECS::Coordinator>& coordinator, const RType::Network::UDPClient& server_infos)
{
    auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
    auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();
    auto event_manager = coordinator->getResource<SFML::EventManager>();
    auto server_entity_manager = coordinator->registerResource<RType::Client::ServerEntityManager>();
    auto draw_sprite = coordinator->getSystem<SFML::DrawSprite>();
    auto transform_sprite = coordinator->getSystem<SFML::TransformSprite>();
    auto update_movement_keys = coordinator->getSystem<SFML::UpdateKeysInput>();
    auto linear_move = coordinator->getSystem<SFML::LinearMove>();
    auto player_bullet = coordinator->getSystem<SFML::PlaySound>();
    auto clock = coordinator->getResource<SFML::Clock>();
    auto keyChecker = coordinator->createEntity();
    auto window = coordinator->getResource<SFML::Window>();
    auto background = coordinator->createEntity();
    auto parallax3 = coordinator->createEntity();
    auto parallax2 = coordinator->createEntity();
    auto parallax1 = coordinator->createEntity();
    RType::PacketManager packetManager;
    coordinator->addComponent<SFML::InputKeys>(keyChecker, SFML::InputKeys());
    SFML::Event event;

    coordinator->addComponent<SFML::SpriteReference>(background, SFML::SpriteReference("background"));
    coordinator->addComponent<SFML::Transform>(background, SFML::Transform({ 0, 0 }, 0, { 5, 5 }));
    coordinator->addComponent<SFML::SpriteReference>(parallax3, SFML::SpriteReference("parallax3"));
    coordinator->addComponent<SFML::Transform>(parallax3, SFML::Transform({ 0, 0 }, 0, { 5, 5 }));
    coordinator->addComponent<SFML::Direction>(parallax3, 0);
    coordinator->addComponent<SFML::Speed>(parallax3, -1);
    coordinator->addComponent<SFML::SpriteReference>(parallax2, SFML::SpriteReference("parallax2"));
    coordinator->addComponent<SFML::Transform>(parallax2, SFML::Transform({ 0, 0 }, 0, { 5, 5 }));
    coordinator->addComponent<SFML::Direction>(parallax2, 0);
    coordinator->addComponent<SFML::Speed>(parallax2, -2.5);
    coordinator->addComponent<SFML::SpriteReference>(parallax1, SFML::SpriteReference("parallax1"));
    coordinator->addComponent<SFML::Transform>(parallax1, SFML::Transform({ 0, 0 }, 0, { 5, 5 }));
    coordinator->addComponent<SFML::Direction>(parallax1, 0);
    coordinator->addComponent<SFML::Speed>(parallax1, -4);
    window->setFramerateLimit(60);
    int32_t gameTm = 0;
    int32_t prevTm = 0;
    clock->restart();
    while (window->isOpen()) {
        while (window->pollEvent(event))
            event_manager->newEvent(event.getEvent());
        while (!udp_handler->isQueueEmpty()) {
            RType::Network::ReceivedPacket packet_received = udp_handler->popElement();
            std::shared_ptr<RType::Network::Header> header = package_manager->decodeHeader(packet_received.packet_data);
            if (!header)
                continue;
            packetManager.choosePacket(coordinator, header->id, packet_received, server_entity_manager);
        }
        if (event_manager->quitEventRegistered())
            window->close();
        transform_sprite->update(coordinator);
        update_movement_keys->update(coordinator);
        sendMovementsKeys(coordinator, server_infos, keyChecker);
        window->clear();
        if (coordinator->getComponent<SFML::Transform>(parallax1).position.getX() <= -1920)
            coordinator->getComponent<SFML::Transform>(parallax1).position = { 0, 0 };
        if (coordinator->getComponent<SFML::Transform>(parallax2).position.getX() <= -1920)
            coordinator->getComponent<SFML::Transform>(parallax2).position = { 0, 0 };
        if (coordinator->getComponent<SFML::Transform>(parallax3).position.getX() <= -1920)
            coordinator->getComponent<SFML::Transform>(parallax3).position = { 0, 0 };
        player_bullet->update(coordinator);
        int32_t tm = clock->getElapsedTime().asMilliseconds();
        int32_t elapsed = tm - prevTm;
        prevTm = tm;
        gameTm += elapsed;
        std::cout << gameTm << std::endl;
        linear_move->update(coordinator, elapsed);
        draw_sprite->update(coordinator);
        window->display();
        event_manager->clear();
    }
    coordinator->destroyEntity(background);
    coordinator->destroyEntity(parallax3);
    coordinator->destroyEntity(parallax2);
    coordinator->destroyEntity(parallax1);
}

/**
 * @brief main is the entry point of the RType Client
 * @param ac Number of command-line arguments
 * @param av Content of command-line arguments
 * @return 0 if the program exited correctly
 */
int main(int ac, char** av)
{
    try {
        auto [server_infos, client_port] = RType::Client::parseArguments(ac, av);
        auto coordinator = std::make_unique<ECS::Coordinator>();

        RType::Client::registerResources(coordinator, client_port);
        RType::Client::registerPackets(coordinator);
        RType::Client::registerComponents(coordinator);
        RType::Client::registerSystems(coordinator);
        RType::Client::loadAssets(coordinator);

        if (RType::Client::display_menu(coordinator) == RType::Client::SceneManager::Scene::QUIT)
            return (0);

        auto package_manager = coordinator->getResource<RType::Network::PackageManager>();
        auto udp_handler = coordinator->getResource<RType::Network::UDPHandler>();

        RType::Packet::PlayerName player_name("AAAAAAAAAAA");
        auto packet = package_manager->createPacket<RType::Packet::PlayerName>(player_name);

        udp_handler->startHandler();
        udp_handler->send(&packet, sizeof(packet), server_infos.getIpAddress(), server_infos.getPort());
        RType::Client::waiting_game_to_start(coordinator);
        RType::Client::game_loop(coordinator, server_infos);
        udp_handler->stopHandler();
    } catch (std::invalid_argument& e) {
        std::cerr
            << "An exception appeared in the argument checking part of the code: "
            << e.what() << std::endl;
        return (84);
    } catch (std::out_of_range& e) {
        std::cerr
            << "An exception appeared in the argument checking part of the code: "
            << "parseArguments: Port should be a number between 0 and 65535"
            << std::endl;
        return (84);
    } catch (ECS::RuntimeException& e) {
        std::cerr << "An exception appeared in the ECS part of the code: "
                  << e.what() << std::endl;
        return (84);
    } catch (...) {
        std::cerr << "An unknown exception appeared" << std::endl;
        return (84);
    }
    return (0);
}