#pragma once

#include <string>
#include "AComponent.hpp"

struct NetworkComponent : public AComponent {
    std::string username;
    NetworkComponent(const std::string &name) : username(name)
    {
    }
};
