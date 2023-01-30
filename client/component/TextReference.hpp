#pragma once

#include <string>

namespace SFML {
    struct TextReference {
        TextReference(const std::string &text_id = "") : id(text_id) {};
        std::string id;
    };
}
