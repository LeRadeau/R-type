#pragma once

#include "Packet.hpp"

namespace Network
{
    class PlayerDisconnectPacket : public Packet {
      public:
        explicit PlayerDisconnectPacket(const std::string &username);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<PlayerDisconnectPacket> deserialize(const std::vector<uint8_t> &data);

        const std::string &getUsername() const;

      private:
        std::string m_username;
    };
} // namespace Network
