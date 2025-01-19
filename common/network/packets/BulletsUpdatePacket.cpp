#include "BulletsUpdatePacket.hpp"
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    BulletsUpdatePacket::BulletsUpdatePacket(const std::vector<BulletData> &data) : m_data(data)
    {
    }

    BulletsUpdatePacket::PacketType BulletsUpdatePacket::getType() const
    {
        return PacketType::BULLETS_UPDATE;
    }

    std::vector<uint8_t> BulletsUpdatePacket::serialize() const
    {
        std::vector<uint8_t> buffer;
        PacketType type = getType();
        size_t length = m_data.size();

        Serializer::serialize<PacketType>(buffer, type);
        Serializer::serialize<size_t>(buffer, length);
        for (size_t i = 0; i < length; i++) {
            Serializer::serialize(buffer, m_data[i].id);
            Serializer::serialize(buffer, m_data[i].x);
            Serializer::serialize(buffer, m_data[i].y);
            Serializer::serialize(buffer, m_data[i].vx);
            Serializer::serialize(buffer, m_data[i].vy);
        }
        return buffer;
    }

    std::unique_ptr<BulletsUpdatePacket> BulletsUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        std::vector<BulletData> buffer;
        size_t vectorLength;
        const uint8_t *indexPtr = data.data();
        size_t length = data.size();

        type = Serializer::deserialize<PacketType>(indexPtr, length);
        if (type != PacketType::BULLETS_UPDATE)
            throw InvalidPacketFormatException("BulletsUpdatePacket should be of type PacketType::BULLETS_UPDATE");
        vectorLength = Serializer::deserialize<size_t>(indexPtr, length);

        for (size_t i = 0; i < vectorLength; i++) {
            BulletData enemy;

            enemy.id = Serializer::deserializeString(indexPtr, length);
            enemy.x = Serializer::deserialize<float>(indexPtr, length);
            enemy.y = Serializer::deserialize<float>(indexPtr, length);
            enemy.vx = Serializer::deserialize<float>(indexPtr, length);
            enemy.vy = Serializer::deserialize<float>(indexPtr, length);
            buffer.push_back(enemy);
        }
        return std::make_unique<BulletsUpdatePacket>(buffer);
    }

    const std::vector<BulletsUpdatePacket::BulletData> &BulletsUpdatePacket::getData() const
    {
        return m_data;
    }

} // namespace Network
