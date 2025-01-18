#pragma once

#include "Packet.hpp"

namespace Network
{
    class PlayerConnectPacket : public Packet {
      public:
        explicit PlayerConnectPacket(const std::string &username, unsigned short udpPort);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<PlayerConnectPacket> deserialize(const std::vector<uint8_t> &data);

        const std::string &getUsername() const;
        unsigned short getPort() const;

      private:
        std::string m_username;
        unsigned short m_udpPort;
    };
} // namespace Network
