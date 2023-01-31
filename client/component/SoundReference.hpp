#pragma once

#include <string>
#include "../sfml/Sound.hpp"

namespace SFML {
    struct SoundReference {
        SoundReference(const std::string &sound_id = "", const Sound::Status &status = Sound::Stopped) : id(sound_id), status(status), first(true) {};
        std::string id;
        Sound::Status status;
        bool first;
    };
}
