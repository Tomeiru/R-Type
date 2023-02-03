#pragma once

#include "../ecs/Coordinator.hpp"

namespace RType::Server {
    void waiting_for_players(std::unique_ptr<ECS::Coordinator> &coordinator);
    void registerPackets(std::unique_ptr<ECS::Coordinator> &coordinator);
    void registerResources(std::unique_ptr<ECS::Coordinator> &coordinator, std::uint16_t port);
    std::uint16_t parseArguments(int ac, char **av);
}