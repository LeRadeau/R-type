#include "EnemyDeathPacket.hpp"
#include <cstring>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{

    EnemyDeathPacket::EnemyDeathPacket(const std::string &enemyId) : m_enemyId(enemyId)
    {
    }

    EnemyDeathPacket::PacketType EnemyDeathPacket::getType() const
    {
        return PacketType::ENEMY_DEATH;
    }

    std::vector<uint8_t> EnemyDeathPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t length = m_enemyId.size();

        if (length == 0)
            throw InvalidPacketFormatException("EnemyDeathPacket should have an enemyId");

        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, m_enemyId);
        return buffer;
    }

    std::unique_ptr<EnemyDeathPacket> EnemyDeathPacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string enemyId;
        PacketType type;
        size_t size = data.size();
        const uint8_t *indexPtr = data.data();

        type = Serializer::deserialize<PacketType>(indexPtr, size);
        if (type != PacketType::ENEMY_DEATH)
            throw InvalidPacketFormatException("EnemyDeathPacket should be of type PacketType::ENEMY_DEATH");
        enemyId = Serializer::deserializeString(indexPtr, size);

        return std::make_unique<EnemyDeathPacket>(enemyId);
    }

    const std::string &EnemyDeathPacket::getEnemyId() const
    {
        return m_enemyId;
    }
} // namespace Network
