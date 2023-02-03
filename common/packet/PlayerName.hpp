#pragma once

#include <string>

namespace RType::Packet {
    struct PlayerName {
        PlayerName(std::string name = "") {
            for (int i = 0; i < 15; i++) {
                _name[i] = (i < name.size() ? name[i] : '\0');
            }
            _name[15] = '\0';
        };
        char _name[16];
    };
}
