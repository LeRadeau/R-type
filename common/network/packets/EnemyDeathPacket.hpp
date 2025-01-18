#pragma once

#include "Packet.hpp"

namespace Network
{
    class EnemyDeathPacket : public Packet {
      public:
        explicit EnemyDeathPacket(const std::string &enemyId);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<EnemyDeathPacket> deserialize(const std::vector<uint8_t> &data);

        const std::string &getEnemyId() const;

      private:
        std::string m_enemyId;
    };
} // namespace Network
