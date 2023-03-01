#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace RType::Map {
    class Map {
        public:
            Map(const std::string &filepath="") {
                std::ifstream f(filepath, std::ios::in);
                if (!f) {
                    _map = "";
                    std::cerr << "Wrong filepath" << std::endl;
                } else {
                    std::string map((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                    _map = map;
                }
            }
            const std::string &getMap() const {
                return _map;
            }
        private:
            std::string _map;
    };
}