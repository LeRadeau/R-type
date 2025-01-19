#pragma once

#include <functional>
#include "ecs/component/AComponent.hpp"
#include "network/packets/Packet.hpp"

struct NetworkCallbackComponent : public AComponent {
    using networkCallback = std::function<void(const std::shared_ptr<Network::Packet> &)>;
    std::unordered_map<Network::Packet::PacketType, networkCallback> callbacks;

    NetworkCallbackComponent()
    {
    }

    void setCallback(Network::Packet::PacketType type, networkCallback fun)
    {
        callbacks.emplace(type, fun);
    }
};
