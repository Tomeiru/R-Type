#pragma once

#include <functional>
#include "../ecs/Coordinator.hpp"

namespace SFML {
    struct Clickable {
        Clickable(const std::function<void(ECS::Coordinator &, ECS::Entity)>& callback = nullptr) : callback(callback) {}
        std::function<void(ECS::Coordinator &, ECS::Entity)> callback;
    };
}
