#pragma once
#include <string>
#include "AComponent.hpp"

struct UsernameComponent : public AComponent {
    std::string username;
    UsernameComponent(const std::string &name) : username(name)
    {
    }
};
