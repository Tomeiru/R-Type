#include <fstream>
#include <cstdlib>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "../common/packet/SpawnEntity.hpp"
#include "./Types.hpp"

using namespace rapidjson;
namespace RType {
    class MapManager {
        public:
            MapManager() {
                std::ifstream ifs("../assets/maps/map.json");
                IStreamWrapper isw(ifs);

                Document d;
                d.ParseStream(isw);
                for (int i = 0; d["enemies"]["easy"].HasMember(std::to_string(i).c_str()); i++) {
                    auto enemy_infos = d["enemies"]["easy"][std::to_string(i).c_str()].GetObject();
                    std::cout << d["enemies"]["easy"][std::to_string(i).c_str()]["dir"].GetInt() << std::endl;
                    setEnemies({
                        enemy_infos["attack"]["speed"].GetUint(),
                        enemy_infos["attack"]["bullet_speed"].GetUint(),
                        enemy_infos["attack"]["dir"].GetUint(),
                        enemy_infos["dir"].GetUint(),
                        enemy_infos["speed"].GetUint(),
                        enemy_infos["life"].GetUint(),
                        enemy_infos["texture_ref"].GetString(),
                        enemy_infos["transform"]["x"].GetFloat(),
                        enemy_infos["transform"]["y"].GetFloat(),
                        enemy_infos["transform"]["rotation"].GetUint(),
                        enemy_infos["transform"]["scale"].GetUint(),
                    });
                    enemy_type.emplace(i, _enemy);
                }
            };

            void setEnemies (Enemy enemy) {
                _enemy.attack.speed = enemy.attack.speed;
                _enemy.attack.bullet_speed = enemy.attack.bullet_speed;
                _enemy.attack.dir = enemy.attack.dir;
                _enemy.dir = enemy.dir;
                _enemy.speed = enemy.speed;
                _enemy.life = enemy.life;
                _enemy.texture_ref = enemy.texture_ref;
                _enemy.transform.x = enemy.transform.x;
                _enemy.transform.y = enemy.transform.y;
                _enemy.transform.rotation = enemy.transform.rotation;
                _enemy.transform.scale = enemy.transform.scale;
            }

            RType::Enemy getEnemyInfosById(std::uint64_t id) {return enemy_type[id];}
        private:
            std::unordered_map<std::uint64_t, Enemy> enemy_type;
            Enemy _enemy;
    };
}