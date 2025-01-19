#pragma once

#include "Notification.hpp"
#include "network/packets/Packet.hpp"

// TODO
class NetworkNotification : public Notification {
  public:
    explicit NetworkNotification(Network::Packet::PacketType)
    {
    }
};
