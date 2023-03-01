#pragma once

#include "../sfml/UDPSocket.hpp"
#include "PackageManager.hpp"
#include "ReceivedPacket.hpp"
#include "ThreadSafeQueue.hpp"
#include "UDPClient.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>

namespace RType::Network {
/**
 * @brief Class handling the udp communication system
 */
class UDPHandler {
public:
    /**
     * @brief Constructs a new UDPHandler object
     *
     * @param port The port to bind the handler to
     * @param package_manager The package manager that will handle the packet registration and creation
     */
    UDPHandler(std::uint16_t port, std::shared_ptr<Network::PackageManager>& package_manager)
        : _port(port)
        , _started(false)
        , _package_manager(package_manager)
    {
    }

    /**
     * @brief Starts the handler
     */
    void startHandler()
    {
        if (_started)
            throw RuntimeException("Server::startHandler", "Handler already started");
        _socket.setBlocking(false);
        if (_socket.bind(_port) != SFML::UDPSocket::Done)
            throw RuntimeException("Server::startHandler", "Binding a port to the handler failed");
        _started = true;
        _thread = std::thread([this] {while (_started) {receive();} });
    }

    /**
     * @brief Stops the handler
     */
    void stopHandler()
    {
        if (!_started)
            throw RuntimeException("Server::stopHandler", "Handler has not been started yet");
        _started = false;
        _socket.unbind();
        _thread.join();
    }

    /**
     * @brief Registers a packet to the package manager
     *
     * @tparam PayloadT The type of the packet
     */
    template<typename PayloadT>
    void registerPacket()
    {
        return _package_manager->registerPacket<PayloadT>();
    }

    /**
     * @brief Receives a packet from the network
     *
     * @return The status of the reception or the size received
     */
    std::uint32_t receive()
    {
        Network::Header header {};
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

    /**
     * @brief Sends a packet to the network
     *
     * @tparam PayloadT The type of the packet
     * @param payload The payload of the packet
     * @param address The address to send the packet to
     * @param port The port to send the packet to
     */
    void send(const void* data, std::size_t size, const SFML::IpAddress& address, uint16_t port)
    {
        if (_socket.send(data, size, address, port) == SFML::UDPSocket::Done)
            std::cout << "UDPHandler::send: data successfully sent" << std::endl;
    }

    /**
     * @brief Creates a packet
     *
     * @tparam PayloadT The type of the packet
     * @param payload The payload of the packet
     * @return The packet
     */
    template<typename PayloadT>
    Network::Packet<PayloadT> createPacket(PayloadT& payload)
    {
        return _package_manager->createPacket(payload);
    }

    /**
     * @brief Checks if the queue is empty
     *
     * @return If the queue is empty
     */
    bool isQueueEmpty()
    {
        return (_queue.empty());
    };

    /**
     * @brief Pops an element from the queue
     *
     * @return The element
     */
    ReceivedPacket popElement()
    {
        return (_queue.pop());
    }

    /**
     * @brief Add a new packet in the list
     *
     * @param packet Packet to add
     */
    void pushQueue(ReceivedPacket& packet)
    {
        _queue.push(packet);
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
