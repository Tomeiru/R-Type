#pragma once

#include "../../ecs/Coordinator.hpp"
#include <functional>

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
    Clickable(const std::function<void(std::unique_ptr<ECS::Coordinator>&, ECS::Entity)>& callback = nullptr)
        : callback(callback)
    {
    }
    std::function<void(std::unique_ptr<ECS::Coordinator>&, ECS::Entity)> callback;
};
}
