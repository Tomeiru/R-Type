#pragma once

#include "../ecs/System.hpp"
#include "../sfml/SoundManager.hpp"
#include "../ecs/Coordinator.hpp"
#include "../component/SoundReference.hpp"

namespace SFML {
    class PlaySound : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            auto sound_manager = coordinator.getResource<SFML::SoundManager>();

            if (!sound_manager->isEnabled())
                return;
            for (const auto &entity: entities) {
                auto &sound_ref = coordinator.getComponent<SoundReference>(entity);
                auto sound = sound_manager->getSound(sound_ref.id);

                if (!sound_ref.first || sound->getStatus() == sound_ref.status) {
                    sound->updateStatus();
                    continue;
                }
                switch (sound_ref.status) {
                    case (Sound::Playing): sound->play(); break;
                    case (Sound::Stopped): sound->stop(); break;
                    default: sound->pause();
                }
                sound_ref.first = false;
            }
        }
    };
}
