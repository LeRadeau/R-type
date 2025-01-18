#pragma once

#include "Packet.hpp"

namespace Network
{
    class EnemiesUpdatePacket : public Packet {
      public:
        struct EnemyData {
            std::string id;
            float x;
            float y;
            int health;
        };
        explicit EnemiesUpdatePacket(const std::vector<EnemyData> &);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<EnemiesUpdatePacket> deserialize(const std::vector<uint8_t> &data);

        const std::vector<EnemyData> &getData() const;

      private:
        std::vector<EnemyData> m_data;
    };
} // namespace Network
