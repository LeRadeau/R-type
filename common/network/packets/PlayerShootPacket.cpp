#include "PlayerShootPacket.hpp"
#include <cstring>
#include <memory>
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayerShootPacket::PlayerShootPacket(const std::string &username, float x, float y)
        : m_username(username), m_x(x), m_y(y)
    {
    }

    PlayerShootPacket::PacketType PlayerShootPacket::getType() const
    {
        return PacketType::PLAYER_SHOOT;
    }
    std::vector<uint8_t> PlayerShootPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t length = m_username.size();
        size_t dataIndex = sizeof(type);

        if (length == 0)
            throw InvalidPacketFormatException("PlayerShootPacket should have a username");
        buffer.resize(sizeof(type) + sizeof(length) + length + sizeof(m_x) + sizeof(m_y));
        std::memcpy(buffer.data(), &type, sizeof(type));
        std::memcpy(buffer.data() + dataIndex, &length, sizeof(length));
        dataIndex += sizeof(length);
        std::memcpy(buffer.data() + dataIndex, m_username.data(), length);
        dataIndex += length;
        std::memcpy(buffer.data() + dataIndex, &m_x, sizeof(m_x));
        dataIndex += sizeof(m_x);
        std::memcpy(buffer.data() + dataIndex, &m_y, sizeof(m_y));
        return buffer;
    }

    std::unique_ptr<PlayerShootPacket> PlayerShootPacket::deserialize(const std::vector<uint8_t> &data)
    {
        size_t length;
        std::string username;
        float x;
        float y;
        size_t dataIndex = sizeof(PacketType);

        if (data.size() <= dataIndex)
            throw IncompletePacketException("PlayerShootPacket contains no data");
        std::memcpy(&length, data.data() + dataIndex, sizeof(size_t));

        dataIndex += sizeof(size_t);

        if (data.size() != length + dataIndex + sizeof(float) + sizeof(float))
            throw IncompletePacketException("PlayerShootPacket username is wrongly sized");

        username.resize(length);
        std::memcpy(username.data(), data.data() + dataIndex, length);
        dataIndex += length;
        std::memcpy(&x, data.data() + dataIndex, sizeof(x));
        dataIndex += sizeof(x);
        std::memcpy(&y, data.data() + dataIndex, sizeof(y));
        return std::make_unique<PlayerShootPacket>(username, x, y);
    }

    const std::string &PlayerShootPacket::getUsername() const
    {
        return m_username;
    }

    float PlayerShootPacket::getX() const
    {
        return m_x;
    }

    float PlayerShootPacket::getY() const
    {
        return m_y;
    }
} // namespace Network
