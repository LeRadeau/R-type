#include "PlayerMovePacket.hpp"
#include <cstring>
#include <memory>
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayerMovePacket::PlayerMovePacket(const std::string &username, float x, float y)
        : m_username(username), m_x(x), m_y(y)
    {
    }

    PlayerMovePacket::PacketType PlayerMovePacket::getType() const
    {
        return PacketType::PLAYER_MOVE;
    }
    std::vector<uint8_t> PlayerMovePacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t length = m_username.size();
        size_t dataIndex = sizeof(type);

        if (length == 0)
            throw InvalidPacketFormatException("PlayerMovePacket should have a username");
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

    std::unique_ptr<PlayerMovePacket> PlayerMovePacket::deserialize(const std::vector<uint8_t> &data)
    {
        size_t length;
        std::string username;
        float x;
        float y;
        size_t dataIndex = sizeof(PacketType);

        if (data.size() <= dataIndex)
            throw IncompletePacketException("PlayerMovePacket contains no data");
        std::memcpy(&length, data.data() + dataIndex, sizeof(size_t));

        dataIndex += sizeof(size_t);

        if (data.size() != length + dataIndex + sizeof(float) + sizeof(float))
            throw IncompletePacketException("PlayerMovePacket username is wrongly sized");

        username.resize(length);
        std::memcpy(username.data(), data.data() + dataIndex, length);
        dataIndex += length;
        std::memcpy(&x, data.data() + dataIndex, sizeof(x));
        dataIndex += sizeof(x);
        std::memcpy(&y, data.data() + dataIndex, sizeof(y));
        return std::make_unique<PlayerMovePacket>(username, x, y);
    }

    const std::string &PlayerMovePacket::getUsername() const
    {
        return m_username;
    }

    float PlayerMovePacket::getX() const
    {
        return m_x;
    }

    float PlayerMovePacket::getY() const
    {
        return m_y;
    }
} // namespace Network
