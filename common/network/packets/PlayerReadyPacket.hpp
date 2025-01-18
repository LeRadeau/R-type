#pragma once

#include "Packet.hpp"

namespace Network
{
    class PlayerReadyPacket : public Packet {
      public:
        explicit PlayerReadyPacket(const std::string &username);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<PlayerReadyPacket> deserialize(const std::vector<uint8_t> &data);

        const std::string &getUsername() const;

      private:
        std::string m_username;
    };
} // namespace Network
