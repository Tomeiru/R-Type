#pragma once

#include "../sfml/IpAddress.hpp"

namespace RType::Network {
/**
 * @brief UDPClient is a class that represents an UDP client
 */
class UDPClient {
public:
    /**
     * @brief Constructs a new UDPClient object
     *
     * @param ip The IP address of the client
     * @param port The port of the client
     */
    UDPClient(const SFML::IpAddress& ip, std::uint16_t port)
        : address(ip)
        , port(port) {};

    /**
     * @brief Get the IP address of the client
     *
     * @return The IP address of the client
     */
    const SFML::IpAddress& getIpAddress() const
    {
        return (address);
    }

    /**
     * @brief Get the port of the client
     *
     * @return The port of the client
     */
    const std::uint16_t& getPort() const
    {
        return (port);
    }

private:
    SFML::IpAddress address;
    std::uint16_t port;
};
}