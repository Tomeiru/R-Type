#pragma once

namespace SFML {
/**
 * @brief Component that determinate if the entity will be destroyed, and when it will be
 */
struct DestroyEntity {
    /**
     * @brief Construct a new DestroyEntity Component
     *
     * @param destroyable Is the entity destroyable
     * @param outWindow Do we destroy the entity when it gets out of the window
     * @param destroySprite Does the entity have a sprite to destroy
     */
    DestroyEntity(bool destroyable = false, bool outWindow = false, bool destroySprite = false)
        : destroyable(destroyable)
        , outWindow(outWindow)
        , sprite(destroySprite) {};
    bool destroyable;
    bool outWindow;
    bool sprite;
};
}