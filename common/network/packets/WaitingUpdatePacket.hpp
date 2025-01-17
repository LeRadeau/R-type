#pragma once

#include "Packet.hpp"

namespace Network
{
    class WaitingUpdatePacket : public Packet {
      public:
        explicit WaitingUpdatePacket(size_t clientTotal);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<WaitingUpdatePacket> deserialize(const std::vector<uint8_t> &data);

        size_t getClientTotal() const;

      private:
        size_t m_clientTotal;

      private:
    };
} // namespace Network
