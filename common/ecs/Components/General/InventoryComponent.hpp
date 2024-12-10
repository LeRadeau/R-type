#pragma once

#include "../Component.hpp"

#include <vector>
#include <string>

struct InventoryComponent : public Component {
    std::vector<std::string> items;

    InventoryComponent(const std::vector<std::string>& items = {})
        : items(items) {}
};
