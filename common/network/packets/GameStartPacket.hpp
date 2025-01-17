#pragma once

#include "Packet.hpp"

namespace Network
{
    class GameStartPacket : public Packet {
      public:
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<GameStartPacket> deserialize(const std::vector<uint8_t> &data);
    };
} // namespace Network
