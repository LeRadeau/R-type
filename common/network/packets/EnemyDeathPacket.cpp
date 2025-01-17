#include "EnemyDeathPacket.hpp"
#include <cstring>
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
        size_t dataIndex = sizeof(type);

        if (length == 0)
            throw InvalidPacketFormatException("EnemyDeathPacket should have an enemyId");

        buffer.resize(sizeof(type) + sizeof(length) + length);
        std::memcpy(buffer.data(), &type, sizeof(type));
        std::memcpy(buffer.data() + dataIndex, &length, sizeof(length));
        dataIndex += sizeof(length);
        std::memcpy(buffer.data() + dataIndex, m_enemyId.data(), length);
        return buffer;
    }

    std::unique_ptr<EnemyDeathPacket> EnemyDeathPacket::deserialize(const std::vector<uint8_t> &data)
    {
        size_t length;
        std::string enemyId;
        size_t dataIndex = sizeof(PacketType);

        if (data.size() <= dataIndex)
            throw IncompletePacketException("EnemyDeathPacket contains no data");
        std::memcpy(&length, data.data() + dataIndex, sizeof(size_t));

        dataIndex += sizeof(size_t);

        if (data.size() != length + dataIndex)
            throw IncompletePacketException("EnemyDeathPacket enemyId is wrongly sized");

        enemyId.resize(length);
        std::memcpy(enemyId.data(), data.data() + dataIndex, length);

        return std::make_unique<EnemyDeathPacket>(enemyId);
    }

    const std::string &EnemyDeathPacket::getEnemyId() const
    {
        return m_enemyId;
    }
} // namespace Network
