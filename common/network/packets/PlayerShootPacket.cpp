#include "PlayerShootPacket.hpp"
#include <cstring>
#include <memory>
#include "network/Serializer.hpp"
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

        if (length == 0)
            throw InvalidPacketFormatException("PlayerShootPacket should have a username");
        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, m_username);
        Serializer::serialize(buffer, m_x);
        Serializer::serialize(buffer, m_y);
        return buffer;
    }

    std::unique_ptr<PlayerShootPacket> PlayerShootPacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string username;
        float x;
        float y;
        size_t length = data.size();
        const uint8_t *indexPtr = data.data();

        PacketType type = Serializer::deserialize<PacketType>(indexPtr, length);
        if (type != PacketType::PLAYER_SHOOT)
            throw InvalidPacketFormatException("PlayerShootPacket should be of type PacketType::PLAYER_SHOOT");
        username = Serializer::deserializeString(indexPtr, length);
        x = Serializer::deserialize<float>(indexPtr, length);
        y = Serializer::deserialize<float>(indexPtr, length);

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
