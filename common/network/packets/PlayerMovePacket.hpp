#pragma once

#include "Packet.hpp"

namespace Network
{

    class PlayerMovePacket : public Packet {
      public:
        explicit PlayerMovePacket(const std::string &username, float x, float y);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<PlayerMovePacket> deserialize(const std::vector<uint8_t> &data);

        const std::string &getUsername() const;
        float getX() const;
        float getY() const;

      private:
        std::string m_username;
        float m_x;
        float m_y;
    };
} // namespace Network
