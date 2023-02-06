#pragma once

#include <string>
#include "../sfml/Music.hpp"

namespace SFML {
    /**
     * @brief Component that allows an entity to play a music
     */
    struct MusicReference {
        /**
         * @brief Construct a new MusicReference Component
         * 
         * @param music_id ID of the music to play
         * @param status Status of the music
         */
        MusicReference(const std::string &music_id = "", const Music::Status &status = Music::Stopped) : id(music_id), status(status), first(true) {};
        std::string id;
        Music::Status status;
        bool first;
    };
}
