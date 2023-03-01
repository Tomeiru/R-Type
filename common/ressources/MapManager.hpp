#pragma once

#include <memory>
#include <unordered_map>
#include "Map.hpp"
#include "../UDPHandler.hpp"
#include "RuntimeException.hpp"
#include "../../ecs/Coordinator.hpp"

namespace RType::Map {
    class MapManager {
        public:
            void registerMap(const std::string &id, const std::string &filename) {
                if (maps.find(id) != maps.end()) {
                    throw RuntimeException("Map: registerMap", "The ID provided has already been registered");
                }
                std::shared_ptr<Map> map = std::make_shared<Map>(filename);
                maps.emplace(id, map);
            };
            std::shared_ptr<Map> getMap(const std::string &id) {
                if (maps.find(id) == maps.end()) {
                    throw RuntimeException("Map: getMap", "The ID provided doesn't match with any map registered");
                }
                return (maps.at(id));
            };
            void spawnWalls(std::shared_ptr<Network::UDPHandler>& udp_handler,
                std::shared_ptr<RType::Network::PackageManager>& package_manager,
                std::unique_ptr<ECS::Coordinator>& coordinator) {
                
            };
        private:
            std::unordered_map<std::string, std::shared_ptr<Map>> maps;
    };
};