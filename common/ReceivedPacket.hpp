#pragma once


#include "UDPClient.hpp"

namespace RType::Network {
    struct ReceivedPacket {
        ReceivedPacket(const SFML::IpAddress &ip, std::uint16_t port, const std::string &packet_data) : sender(ip, port), packet_data(packet_data) {}
        ReceivedPacket(const UDPClient &sender, const std::string &packet_data) : sender(sender), packet_data(packet_data) {}
        UDPClient sender;
        std::string packet_data;
    };
}