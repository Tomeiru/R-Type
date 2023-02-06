#pragma once

#include <string>

namespace SFML {
/**
 * @brief Component that allows an entity to have a text
 */
struct TextReference {
    /**
     * @brief Construct a new TextReference Component
     *
     * @param text_id ID of the text to apply
     */
    TextReference(const std::string& text_id = "")
        : id(text_id) {};
    std::string id;
};
}
