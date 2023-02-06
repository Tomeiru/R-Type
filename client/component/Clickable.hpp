#pragma once

#include <functional>
#include "../ecs/Coordinator.hpp"

namespace SFML {
    /**
     * @brief Component that allows an entity to be clicked
     */
    struct Clickable {
        /**
         * @brief Construct a new Clickable Component
         * 
         * @param callback Callback function to be called when the entity is clicked
         */
        Clickable(const std::function<void(ECS::Coordinator &, ECS::Entity)>& callback = nullptr) : callback(callback) {}
        std::function<void(ECS::Coordinator &, ECS::Entity)> callback;
    };
}
