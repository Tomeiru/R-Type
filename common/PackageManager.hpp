#pragma once

#include <unordered_map>
#include <string>
#include <cstring>
#include <memory>
#include "RuntimeException.hpp"

#define NETWORK_MAGIC 0xDEADBEEF

namespace RType::Network {
    struct Header {
        unsigned int magic;
        char id;
        unsigned int created_at;
    };

    template<typename PayloadT>
    struct Packet {
        Header header;
        PayloadT payload;
    };

    class PackageManager {
    public:
        template<typename PayloadT>
        Packet<PayloadT> createPacket(PayloadT &payload) {
            std::string data_type = typeid(PayloadT).name();
            auto type_id = _typename_to_id.find(data_type);

            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::createPacket", "Packet Type has not been registered yet");
            Packet<PayloadT> packet = {{NETWORK_MAGIC, type_id->second, 0}, payload};
            return packet;
        }

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

        std::string getTypeName(char id) const {
            auto type_name =  _id_to_typename.find(id);
            if (type_name == _id_to_typename.end())
                throw RuntimeException("PackageManager::getTypeName", "Packet Type has not been registered yet");
            return (type_name->second);
        }

        template<typename PayloadT>
        char getTypeId() const {
            std::string name = typeid(PayloadT).name();
            auto type_id = _typename_to_id.find(name);
            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::getTypeId", "Packet Type has not been registered yet");
            return (type_id->second);
        }

        char getTypeId(const std::string &name) const {
            auto type_id = _typename_to_id.find(name);
            if (type_id == _typename_to_id.end())
                throw RuntimeException("PackageManager::getTypeId", "Packet Type has not been registered yet");
            return (type_id->second);
        }

        std::shared_ptr<Header> decodeHeader(const std::string &data) {
            Header header{};
            char *raw_data = const_cast<char *>(data.c_str());

            std::memcpy(&header, raw_data, sizeof(Header));
            if (header.magic != NETWORK_MAGIC || _id_to_typename.find(header.id) ==_id_to_typename.end())
                return nullptr;
            return (std::make_shared<Header>(header));
        }

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
    };
}
