#pragma once
#include <string>
#include "AComponent.hpp"

struct usernameComponent : public AComponent {
    std::string username;
    usernameComponent(const std::string &name) : username(name)
    {
    }
};
