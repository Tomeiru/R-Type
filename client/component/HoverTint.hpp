#pragma once

#include <string>

namespace SFML {

    struct HoverTint {
        HoverTint(const std::string &on_id = "", const std::string &off_id = "") : on_id(on_id), off_id(off_id) {};
        std::string on_id;
        std::string off_id;
    };

}
