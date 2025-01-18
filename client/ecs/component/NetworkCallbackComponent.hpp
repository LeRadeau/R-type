#pragma once

#include <functional>
#include "ecs/component/AComponent.hpp"
#include "network_types.hpp"

struct NetworkCallbackComponent : public AComponent {
    using networkCallback = std::function<void(const char *&)>;
    std::unordered_map<MessageType, networkCallback> callbacks;

    NetworkCallbackComponent()
    {
    }

    void setCallback(MessageType type, networkCallback fun)
    {
        callbacks[type] = fun;
    }
};
