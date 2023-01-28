#pragma once

#include "../../ecs/Coordinator.hpp"

namespace RType {
    namespace Server {
        class SystemExample : ECS::System {
            void update(ECS::Coordinator &coordinator) {
                for (const auto &entity: entities);
            }
        };
    }
}
