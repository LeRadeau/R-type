#include "PlayerMovePacket.hpp"
#include <cstring>
#include <memory>
#include "network/Serializer.hpp"
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

        if (length == 0)
            throw InvalidPacketFormatException("PlayerMovePacket should have a username");
        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, m_username);
        Serializer::serialize(buffer, m_x);
        Serializer::serialize(buffer, m_y);
        return buffer;
    }

    std::unique_ptr<PlayerMovePacket> PlayerMovePacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string username;
        float x;
        float y;
        PacketType type;
        size_t size = data.size();
        const uint8_t *indexPtr = data.data();

        type = Serializer::deserialize<PacketType>(indexPtr, size);
        if (type != PacketType::PLAYER_MOVE)
            throw InvalidPacketFormatException("PlayerMovePacket should be of type PacketType::PLAYER_MOVE");
        username = Serializer::deserializeString(indexPtr, size);
        x = Serializer::deserialize<float>(indexPtr, size);
        y = Serializer::deserialize<float>(indexPtr, size);
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
