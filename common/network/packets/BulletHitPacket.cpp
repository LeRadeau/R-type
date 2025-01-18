#include "BulletHitPacket.hpp"
#include <cstring>
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
        size_t dataIndex = sizeof(type);

        if (length == 0)
            throw InvalidPacketFormatException("BulletHitPacket should have a bulletId");

        buffer.resize(sizeof(type) + sizeof(length) + length);
        std::memcpy(buffer.data(), &type, sizeof(type));
        std::memcpy(buffer.data() + dataIndex, &length, sizeof(length));
        dataIndex += sizeof(length);
        std::memcpy(buffer.data() + dataIndex, m_bulletId.data(), length);
        return buffer;
    }

    std::unique_ptr<BulletHitPacket> BulletHitPacket::deserialize(const std::vector<uint8_t> &data)
    {
        size_t length;
        std::string bulletId;
        size_t dataIndex = sizeof(PacketType);

        if (data.size() <= dataIndex)
            throw IncompletePacketException("BulletHitPacket contains no data");
        std::memcpy(&length, data.data() + dataIndex, sizeof(size_t));

        dataIndex += sizeof(size_t);

        if (data.size() != length + dataIndex)
            throw IncompletePacketException("BulletHitPacket bulletId is wrongly sized");

        bulletId.resize(length);
        std::memcpy(bulletId.data(), data.data() + dataIndex, length);

        return std::make_unique<BulletHitPacket>(bulletId);
    }

    const std::string &BulletHitPacket::getBulletId() const
    {
        return m_bulletId;
    }

} // namespace Network
