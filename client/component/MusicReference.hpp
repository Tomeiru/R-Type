#pragma once

#include <string>
#include "../sfml/Music.hpp"

namespace SFML {
    struct MusicReference {
        MusicReference(const std::string &music_id = "", const Music::Status &status = Music::Stopped) : id(music_id), status(status), first(true) {};
        std::string id;
        Music::Status status;
        bool first;
    };
}
