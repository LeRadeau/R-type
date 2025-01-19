#include "BulletHitPacket.hpp"
#include <cstring>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    BulletHitPacket::BulletHitPacket(const std::string &bulletId) : m_bulletId(bulletId)
    {
    }

    BulletHitPacket::PacketType BulletHitPacket::getType() const
    {
        return PacketType::BULLET_HIT;
    }

    std::vector<uint8_t> BulletHitPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t length = m_bulletId.size();

        if (length == 0)
            throw InvalidPacketFormatException("BulletHitPacket should have a bulletId");

        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, m_bulletId);
        return buffer;
    }

    std::unique_ptr<BulletHitPacket> BulletHitPacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string bulletId;
        PacketType type;
        size_t size = data.size();
        const uint8_t *indexPtr = data.data();

        type = Serializer::deserialize<PacketType>(indexPtr, size);
        if (type != PacketType::BULLET_HIT)
            throw InvalidPacketFormatException("BulletHitPacket should be of type PacketType::BULLET_HIT");
        bulletId = Serializer::deserializeString(indexPtr, size);
        return std::make_unique<BulletHitPacket>(bulletId);
    }

    const std::string &BulletHitPacket::getBulletId() const
    {
        return m_bulletId;
    }

} // namespace Network
