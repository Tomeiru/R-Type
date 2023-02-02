#pragma once

#include <cstdint>
#include <thread>
#include <SFML/Network/UdpSocket.hpp>
#include <iostream>
#include "PackageManager.hpp"
#include "ThreadSafeQueue.hpp"

namespace RType::Network {
    /**
     * @brief Class which is used to handle UDP packets
     */
    class UDPHandler {
    public:
        /**
         * @brief Constructor of the UDPHandler
         * @param port Port to bind
         * @param package_manager Package manager
         */
        UDPHandler(std::uint16_t port, const Network::PackageManager &package_manager) : _port(port), _started(false), _package_manager(package_manager) {}
        void startHandler() {
            if (_started)
                throw RuntimeException("Server::startHandler", "Handler already started");
            _socket.setBlocking(false);
            if (_socket.bind(_port) != sf::Socket::Done)
                throw RuntimeException("Server::startHandler", "Binding a port to the handler failed");
            _started = true;
            _thread = std::thread([this] {while (_started) {receive();}});
        }
        /**
         * @brief Stop the handler
         */
        void stopHandler() {
            if (!_started)
                throw RuntimeException("Server::stopHandler", "Handler has not been started yet");
            _started = false;
            _socket.unbind();
            _thread.join();
        }

        /**
         * @brief Register a packet
         * @tparam PayloadT Type of the payload
         */
        template<typename PayloadT>
        void registerPacket() {
            return _package_manager.registerPacket<PayloadT>();
        }

        /**
         * @brief Receive a packet
         * @return Status of the reception
         */
        std::uint32_t receive() {
            Network::Header header{};
            sf::IpAddress sender;
            std::uint16_t port;
            std::size_t received;
            char data[1024];
            auto status = _socket.receive(data, 1024, received, sender, port);

            if (status != sf::Socket::Done) {
                return status;
            }
            std::string data_received(data, received);
            _queue.push(data);
            std::cout << "A packed has been received" << std::endl;
            return received;
        }

        /**
         * @brief Send a packet
         * @tparam PayloadT Type of the payload
         * @param packet Packet to send
         * @param address Address to send the packet to
         * @param port Port to send the packet to
         */
        void send(const void *data, std::size_t size, const sf::IpAddress &address, uint16_t port) {
            if (_socket.send(data, size, address, port) == sf::Socket::Done)
                std::cout << "UDPHandler::send: data successfully sent" <<  std::endl;
        }

        /**
         * @brief Create a packet
         * @tparam PayloadT Type of the payload
         * @param payload Payload of the packet
         * @return Packet created
         */
        template<typename PayloadT>
        Network::Packet<PayloadT> createPacket(PayloadT &payload) {
            return _package_manager.createPacket(payload);
        }

    private:
        const Network::PackageManager &_package_manager;
        std::thread _thread;
        sf::UdpSocket _socket;
        std::uint16_t _port;
        Network::ThreadSafeQueue<std::string> _queue;
        bool _started;
    };
}
