#pragma once

#include "../sfml/Sound.hpp"
#include <string>

namespace SFML {
/**
 * @brief Component that allows an entity to play a sound
 */
struct SoundReference {
    /**
     * @brief Construct a new SoundReference Component
     *
     * @param sound_id ID of the sound to play
     * @param status Status of the sound
     */
    SoundReference(const std::string& sound_id = "", const Sound::Status& status = Sound::Stopped)
        : id(sound_id)
        , status(status)
        , first(true) {};
    std::string id;
    Sound::Status status;
    bool first;
};
}
