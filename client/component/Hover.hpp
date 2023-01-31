#pragma once

namespace SFML {
    struct Hover {
        Hover(bool hovered = false) : hovered(hovered) {};
        bool hovered;
    };
}
