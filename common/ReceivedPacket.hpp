#pragma once


#include "UDPClient.hpp"

namespace RType::Network {
    /**
     * @brief ReceivedPacket is a class that represents a received packet
     */
    struct ReceivedPacket {
        /**
         * @brief Constructs a new ReceivedPacket object
         * @param ip The IP address of the sender
         * @param port The port of the sender
         * @param packet_data The data of the packet
         */
        ReceivedPacket(const SFML::IpAddress &ip, std::uint16_t port, const std::string &packet_data) : sender(ip, port), packet_data(packet_data) {}

        /**
         * @brief Constructs a new ReceivedPacket object
         * @param sender The sender of the packet
         * @param packet_data The data of the packet
         */
        ReceivedPacket(const UDPClient &sender, const std::string &packet_data) : sender(sender), packet_data(packet_data) {}
        UDPClient sender;
        std::string packet_data;
    };
}