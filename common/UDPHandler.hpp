#pragma once

#include <cstdint>
#include <thread>
#include "../sfml/UDPSocket.hpp"
#include <iostream>
#include <memory>
#include "PackageManager.hpp"
#include "ThreadSafeQueue.hpp"
#include "UDPClient.hpp"
#include "ReceivedPacket.hpp"

namespace RType::Network {
    class UDPHandler {
    public:
        UDPHandler(std::uint16_t port, std::shared_ptr<Network::PackageManager> package_manager) : _port(port), _started(false), _package_manager(package_manager) {}
        void startHandler() {
            if (_started)
                throw RuntimeException("Server::startHandler", "Handler already started");
            _socket.setBlocking(false);
            if (_socket.bind(_port) != SFML::UDPSocket::Done)
                throw RuntimeException("Server::startHandler", "Binding a port to the handler failed");
            _started = true;
            _thread = std::thread([this] {while (_started) {receive();}});
        }
        void stopHandler() {
            if (!_started)
                throw RuntimeException("Server::stopHandler", "Handler has not been started yet");
            _started = false;
            _socket.unbind();
            _thread.join();
        }

        template<typename PayloadT>
        void registerPacket() {
            return _package_manager->registerPacket<PayloadT>();
        }

        std::uint32_t receive() {
            Network::Header header{};
            SFML::IpAddress sender(0);
            std::uint16_t port;
            std::size_t received;
            char data[1024];
            auto status = _socket.receive(data, 1024, received, sender, port);

            if (status != SFML::UDPSocket::Done) {
                return status;
            }
            std::string data_received(data, received);
            ReceivedPacket received_packet(sender, port, data_received);
            _queue.push(received_packet);
            std::cout << "A packed has been received" << std::endl;
            return received;
        }

        void send(const void *data, std::size_t size, const SFML::IpAddress &address, uint16_t port) {
            if (_socket.send(data, size, address, port) == SFML::UDPSocket::Done)
                std::cout << "UDPHandler::send: data successfully sent" <<  std::endl;
        }

        template<typename PayloadT>
        Network::Packet<PayloadT> createPacket(PayloadT &payload) {
            return _package_manager->createPacket(payload);
        }

        bool isQueueEmpty() {
            return (_queue.empty());
        };

        ReceivedPacket popElement() {
            return (_queue.pop());
        }

    private:
        std::shared_ptr<Network::PackageManager> _package_manager;
        std::thread _thread;
        SFML::UDPSocket _socket;
        std::uint16_t _port;
        Network::ThreadSafeQueue<ReceivedPacket> _queue;
        bool _started;
    };
}
