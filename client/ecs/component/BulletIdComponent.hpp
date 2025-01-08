#pragma once
#include <string>
#include "AComponent.hpp"

struct BulletIdComponent : public AComponent {
    std::string id;
    BulletIdComponent(const std::string &id) : id(id)
    {
    }
};
