#pragma once

#include "ecs/Entity.hpp"
#include "network/packets/Packet.hpp"
namespace NetworkCallbacks
{
    void onWaitUpdateClientNbr(const std::shared_ptr<Network::Packet> &packet, Entity &textField);
} // namespace NetworkCallbacks
