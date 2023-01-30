#pragma once

#include "../ecs/System.hpp"
#include "../ecs/Coordinator.hpp"
#include "../sfml/MusicManager.hpp"
#include "../component/MusicReference.hpp"

namespace SFML {
    class PlayMusic : public ECS::System {
    public:
        void update(ECS::Coordinator &coordinator) {
            auto music_manager = coordinator.getResource<SFML::MusicManager>();

            if (!music_manager->isEnabled())
                return;
            for (const auto &entity: entities) {
                auto &music_ref = coordinator.getComponent<MusicReference>(entity);
                auto music = music_manager->getMusic(music_ref.id);

                if (!music_ref.first || music->getStatus() == music_ref.status) {
                    music->updateStatus();
                    continue;
                }
                switch (music_ref.status) {
                    case (Music::Playing): music->play(); break;
                    case (Music::Stopped): music->stop(); break;
                    default: music->pause();
                }
                music_ref.first = false;
            }
        }
    };
}
