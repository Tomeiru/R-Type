#pragma once

#include "../../ecs/Coordinator.hpp"
#include "../../ecs/System.hpp"
#include "../../sfml/MusicManager.hpp"
#include "../component/MusicReference.hpp"

namespace SFML {
/**
 * @brief System that plays all musics
 */
class PlayMusic : public ECS::System {
public:
    /**
     * @brief Function that plays all musics
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(std::unique_ptr<ECS::Coordinator>& coordinator)
    {
        auto music_manager = coordinator->getResource<SFML::MusicManager>();

        if (!music_manager->isEnabled())
            return;
        for (const auto& entity : entities) {
            auto& music_ref = coordinator->getComponent<MusicReference>(entity);
            auto music = music_manager->getMusic(music_ref.id);

            if (!music_ref.first || music->getStatus() == music_ref.status) {
                music->updateStatus();
                continue;
            }
            switch (music_ref.status) {
            case (Music::Playing):
                music->play();
                break;
            case (Music::Stopped):
                music->stop();
                break;
            default:
                music->pause();
            }
            music_ref.first = false;
        }
    }
};
}
