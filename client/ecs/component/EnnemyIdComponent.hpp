#pragma once
#include <string>
#include "AComponent.hpp"

struct EnnemyIdComponent : public AComponent {
    std::string id;
    EnnemyIdComponent(const std::string &id) : id(id)
    {
    }
};
