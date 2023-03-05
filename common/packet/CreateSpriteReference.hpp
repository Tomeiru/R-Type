#pragma once

#include <string>
namespace RType::Packet {
/**
 * @brief CreateSpriteReference is a class that creates a sprite from an other sprite
 */
struct CreateSpriteReference {
    /**
     * @brief Construct a new CreateSpriteReference object
     *
     * @param spriteId The sprite to create
     * @param linkSprite The sprite to refer to
     */
    CreateSpriteReference(std::string spriteId = "", std::string linkSprite = "")
    {
        for (int i = 0; i < 31; i++) {
            _spriteId[i] = (i < spriteId.size() ? spriteId[i] : '\0');
        }
        _spriteId[31] = '\0';
        for (int i = 0; i < 31; i++) {
            _linkSprite[i] = (i < linkSprite.size() ? linkSprite[i] : '\0');
        }
        _linkSprite[31] = '\0';
    };

    char _spriteId[32];
    char _linkSprite[32];
};
}