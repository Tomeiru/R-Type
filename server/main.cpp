#include <iostream>

#include "../ecs/RuntimeException.hpp"
#include "../common/PackageManager.hpp"
#include "../common/UDPHandler.hpp"
#include "../ecs/Coordinator.hpp"

std::uint16_t parseArguments(int ac, char **av) {
    if (ac != 2)
        throw std::invalid_argument("parseArguments: Wrong number of argument (2 arguments expected)");
    std::uint64_t port = std::stoul(av[1]);
    if (port > 65535)
        throw std::invalid_argument("parseArguments: Port should be a number between 0 and 65535");
    return (port);
}

int main(int ac, char **av)
{
    try {
        std::uint16_t port = parseArguments(ac, av);
        ECS::Coordinator coordinator;
        auto package_manager = coordinator.registerResource<RType::Network::PackageManager>();
        auto udp_handler = coordinator.registerResource<RType::Network::UDPHandler>(port, package_manager);
        udp_handler->startHandler();

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
    catch (...) {
        std::cerr << "An unknown exception appeared" << std::endl;
        return (84);
    }

    return (0);
}