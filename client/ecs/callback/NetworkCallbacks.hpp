#pragma once

#include "ecs/Entity.hpp"
namespace NetworkCallbacks
{
    void onWaitUpdateClientNbr(const char *&packet, Entity &textField);
} // namespace NetworkCallbacks
