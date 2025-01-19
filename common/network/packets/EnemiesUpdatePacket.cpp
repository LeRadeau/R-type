
#include "EnemiesUpdatePacket.hpp"
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    EnemiesUpdatePacket::EnemiesUpdatePacket(const std::vector<EnemyData> &data) : m_data(data)
    {
    }

    EnemiesUpdatePacket::PacketType EnemiesUpdatePacket::getType() const
    {
        return PacketType::ENEMIES_UPDATE;
    }

    std::vector<uint8_t> EnemiesUpdatePacket::serialize() const
    {
        std::vector<uint8_t> buffer;
        PacketType type = getType();
        size_t length = m_data.size();

        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, length);
        for (size_t i = 0; i < length; i++) {
            Serializer::serialize(buffer, m_data[i].id);
            Serializer::serialize(buffer, m_data[i].x);
            Serializer::serialize(buffer, m_data[i].y);
            Serializer::serialize(buffer, m_data[i].health);
        }
        return buffer;
    }

    std::unique_ptr<EnemiesUpdatePacket> EnemiesUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        std::vector<EnemyData> buffer;
        size_t vectorLength;
        size_t length = data.size();
        const uint8_t *indexPtr = data.data();

        type = Serializer::deserialize<PacketType>(indexPtr, length);
        if (type != PacketType::ENEMIES_UPDATE)
            throw InvalidPacketFormatException("EnemiesUpdatePacket should be of type PacketType::ENEMIES_UPDATE");

        vectorLength = Serializer::deserialize<size_t>(indexPtr, length);
        for (size_t i = 0; i < vectorLength; i++) {
            EnemyData enemy;
            enemy.id = Serializer::deserializeString(indexPtr, length);
            enemy.x = Serializer::deserialize<float>(indexPtr, length);
            enemy.y = Serializer::deserialize<float>(indexPtr, length);
            enemy.health = Serializer::deserialize<int>(indexPtr, length);
            buffer.push_back(enemy);
        }
        return std::make_unique<EnemiesUpdatePacket>(buffer);
    }

    const std::vector<EnemiesUpdatePacket::EnemyData> &EnemiesUpdatePacket::getData() const
    {
        return m_data;
    }
} // namespace Network
