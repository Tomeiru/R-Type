#pragma once

#include "../component/SoundReference.hpp"
#include "../ecs/Coordinator.hpp"
#include "../ecs/System.hpp"
#include "../sfml/SoundManager.hpp"

namespace SFML {
/**
 * @brief System that plays all sounds
 */
class PlaySound : public ECS::System {
public:
    /**
     * @brief Function that plays all sounds
     *
     * @param coordinator Reference to the ecs coordinator
     */
    void update(ECS::Coordinator& coordinator)
    {
        auto sound_manager = coordinator.getResource<SFML::SoundManager>();

        if (!sound_manager->isEnabled())
            return;
        for (const auto& entity : entities) {
            auto& sound_ref = coordinator.getComponent<SoundReference>(entity);
            auto sound = sound_manager->getSound(sound_ref.id);

            if (!sound_ref.first || sound->getStatus() == sound_ref.status) {
                sound->updateStatus();
                continue;
            }
            switch (sound_ref.status) {
            case (Sound::Playing):
                sound->play();
                break;
            case (Sound::Stopped):
                sound->stop();
                break;
            default:
                sound->pause();
            }
            sound_ref.first = false;
        }
    }
};
}
