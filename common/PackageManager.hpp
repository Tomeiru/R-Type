#pragma once

#include <unordered_map>
#include <string>
#include <cstring>
#include <memory>
#include "RuntimeException.hpp"

#define NETWORK_MAGIC 0xDEADBEEF

namespace RType::Network {
    /**
     * @brief Header is a struct that represents a packet header
     */
    struct Header {
        unsigned int magic;
        char id;
        unsigned short player_id;
        unsigned int created_at;
    };

    /**
     * @brief Packet is a struct that represents a packet
     * @tparam PayloadT The type of the payload
     */
    template<typename PayloadT>
    struct Packet {
        Header header;
        PayloadT payload;
    };

    /**
     * @brief PackageManager is a class that manages the packets
     */
    class PackageManager {
    public:
        /**
         * @brief Creates a packet of PayloadT type
         * @tparam PayloadT The type of the payload
         * @param payload The payload of the packet
         * @return The packet
         */
        template<typename PayloadT>
        Packet<PayloadT> createPacket(PayloadT &payload, std::uint16_t player_id = 0) {
            std::string data_type = typeid(PayloadT).name();
            auto type_id = _typename_to_id.find(data_type);

            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::createPacket", "Packet Type has not been registered yet");
            Packet<PayloadT> packet = {{NETWORK_MAGIC, type_id->second, player_id, 0}, payload};
            return packet;
        }

        /**
         * @brief Registers a packet Type to the package manager
         * @tparam PayloadT The type of the payload
         */
        template<typename PayloadT>
        void registerPacket() {
            std::string data_type = typeid(PayloadT).name();

            if (_typename_to_id.find(data_type) != _typename_to_id.end())
                throw RuntimeException("PackageManager::registerPacket", "Packet Type has already been registered yet");
            _typename_to_id.emplace(data_type, _id);
            _id_to_typename.emplace(_id, data_type);
            _id++;
        }

        /**
         * @brief Gets the type name of a packet from its TypeId
         * @param id The id of the packet
         * @return The type name of the packet
         */
        std::string getTypeName(char id) const {
            auto type_name =  _id_to_typename.find(id);
            if (type_name == _id_to_typename.end())
                throw RuntimeException("PackageManager::getTypeName", "Packet Type has not been registered yet");
            return (type_name->second);
        }

        /**
         * @brief Gets the TypeId of a packet from its type
         * @tparam PayloadT The type of the payload
         * @return The id of the packet
         */
        template<typename PayloadT>
        char getTypeId() const {
            std::string name = typeid(PayloadT).name();
            auto type_id = _typename_to_id.find(name);
            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::getTypeId", "Packet Type has not been registered yet");
            return (type_id->second);
        }

        /**
         * @brief Gets the TypeId of a packet from its type name
         * @param name The name of the packet
         * @return The id of the packet
         */
        char getTypeId(const std::string &name) const {
            auto type_id = _typename_to_id.find(name);
            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::getTypeId", "Packet Type has not been registered yet");
            return (type_id->second);
        }

        /**
         * @brief Decodes the packet header from its data in string
         * @param data The data of the packet
         * @return The header of the packet
         */
        std::shared_ptr<Header> decodeHeader(const std::string &data) {
            Header header{};
            char *raw_data = const_cast<char *>(data.c_str());

            std::memcpy(&header, raw_data, sizeof(Header));
            if (header.magic != NETWORK_MAGIC || _id_to_typename.find(header.id) ==_id_to_typename.end())
                return nullptr;
            return (std::make_shared<Header>(header));
        }

        /**
         * @brief Decodes the packet payload from its data in string
         * @tparam PayloadT The type of the payload
         * @param data The data of the packet
         * @return The payload of the packet
         */
        template<typename PayloadT>
        std::shared_ptr<PayloadT> decodeContent(const std::string &data) {
            PayloadT payload;
            char *raw_data = const_cast<char *>(data.c_str()) + sizeof(Header);

            std::memcpy(&payload, raw_data, sizeof(PayloadT));
            return (std::make_shared<PayloadT>(payload));
        }

    private:
        std::unordered_map<std::string, char> _typename_to_id;
        std::unordered_map<char, std::string> _id_to_typename;
        std::uint8_t _id = 0;
    };
}
