#pragma once

#include "../Component.hpp"
#include <string>

struct BindRemoteComponent : public Component {
    public:
        std::string ipClient;
        unsigned short portClient;
};