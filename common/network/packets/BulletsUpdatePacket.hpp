#pragma once

#include "Packet.hpp"

namespace Network
{
    class BulletsUpdatePacket : public Packet {
      public:
        struct BulletData {
            std::string id;
            float x;
            float y;
            float vx;
            float vy;
        };
        BulletsUpdatePacket(const std::vector<BulletData> &);

        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<BulletsUpdatePacket> deserialize(const std::vector<uint8_t> &data);

        const std::vector<BulletData> &getData() const;

      private:
        std::vector<BulletData> m_data;
    };
} // namespace Network
