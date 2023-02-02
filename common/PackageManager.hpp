#pragma once

#include <unordered_map>
#include <string>
#include "RuntimeException.hpp"

#define NETWORK_MAGIC 0xDEADBEEF

namespace RType::Network {
    /**
     * @brief Header of a packet which is sent over the network
     */
    struct Header {
        unsigned int magic;
        char id;
        unsigned int created_at;
    };

    /**
     * @brief Packet which is sent over the network
     * @tparam PayloadT Type of the payload
     */
    template<typename PayloadT>
    struct Packet {
        Header header;
        PayloadT payload;
    };

    /**
     * @brief Package manager which is used to register and create packets
     */
    class PackageManager {
    public:
        /**
         * @brief Create a packet with the given payload
         * @tparam PayloadT Type of the payload
         * @param payload Payload which contains the data
         * @return Packet created
         */
        template<typename PayloadT>
        Packet<PayloadT> createPacket(PayloadT &payload) {
            std::string data_type = typeid(PayloadT).name();
            auto type_id = _typename_to_id.find(data_type);

            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::createPacket", "Packet Type has not been registered yet");
            Packet<PayloadT> packet = {{NETWORK_MAGIC, type_id->second, 0}, payload};
            return packet;
        }

        /**
         * @brief Register a payload type
         * @tparam PayloadT Type of the payload
         */
        template<typename PayloadT>
        void registerPacket() {
            static unsigned int id = 0;
            std::string data_type = typeid(PayloadT).name();

            if (_typename_to_id.find(data_type) != _typename_to_id.end())
                throw RuntimeException("PackageManager::registerPacket", "Packet Type has already been registered yet");
            _typename_to_id.emplace(data_type, id);
            _id_to_typename.emplace(id, data_type);
            id++;
        }

        /**
         * @brief Get the type name of the given id
         * @param id Id of the type
         * @return Type name
         */
        std::string getTypeName(char id) const {
            auto type_name =  _id_to_typename.find(id);
            if (type_name == _id_to_typename.end())
                throw RuntimeException("PackageManager::getTypeName", "Packet Type has not been registered yet");
            return (type_name->second);
        }

        /**
         * @brief Get the type id of the given name
         * @param name Name of the type
         * @return Type id
         */
        char getTypeId(const std::string &name) const {
            auto type_id = _typename_to_id.find(name);
            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::getTypeId", "Packet Type has not been registered yet");
            return (type_id->second);
        }

    private:
        std::unordered_map<std::string, char> _typename_to_id;
        std::unordered_map<char, std::string> _id_to_typename;
    };
}
