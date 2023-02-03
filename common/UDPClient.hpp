#pragma once


#include "../sfml/IpAddress.hpp"

namespace RType::Network {
    class UDPClient {
        public:
            UDPClient(const SFML::IpAddress &ip, std::uint16_t port) : address(ip), port(port) {};
            const SFML::IpAddress &getIpAddress() const {
                return (address);
            }
            const std::uint16_t &getPort() const {
                return (port);
            }
        private:
            SFML::IpAddress address;
            std::uint16_t port;
    };
}