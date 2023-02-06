#pragma once

#include <string>

namespace SFML {
    /**
     * @brief Component that allows an entity to have a tint when hovered
     */
    struct HoverTint {
        /**
         * @brief Construct a new HoverTint Component
         * 
         * @param on_id ID of the tint to apply when the entity is hovered
         * @param off_id ID of the tint to apply when the entity is not hovered
         */
        HoverTint(const std::string &on_id = "", const std::string &off_id = "") : on_id(on_id), off_id(off_id) {};
        std::string on_id;
        std::string off_id;
    };

}
