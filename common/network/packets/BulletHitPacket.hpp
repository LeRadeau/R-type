#pragma once

#include "Packet.hpp"

namespace Network
{
    class BulletHitPacket : public Packet {
      public:
        explicit BulletHitPacket(const std::string &bulletId);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<BulletHitPacket> deserialize(const std::vector<uint8_t> &data);

        const std::string &getBulletId() const;

      private:
        std::string m_bulletId;
    };
} // namespace Network
