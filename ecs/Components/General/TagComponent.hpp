#pragma once

#include "../Component.hpp"
#include <string>

struct TagComponent : public Component {
    std::string tag; // Ex: "player", "enemy"
    explicit TagComponent(const std::string& tag = "") : tag(tag) {}
};
