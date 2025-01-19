#include "PlayerReadyPacket.hpp"
#include <cstring>
#include <memory>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayerReadyPacket::PlayerReadyPacket(const std::string &username) : m_username(username)
    {
    }

    PlayerReadyPacket::PacketType PlayerReadyPacket::getType() const
    {
        return PacketType::PLAYER_READY;
    }

    std::vector<uint8_t> PlayerReadyPacket::serialize() const
    {
        std::vector<uint8_t> buffer;
        PacketType type = getType();
        size_t length = m_username.size();

        if (length == 0)
            throw InvalidPacketFormatException("PlayerReadyPacket should have a username");

        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, length);
        Serializer::serialize(buffer, m_username);
        return buffer;
    }

    std::unique_ptr<PlayerReadyPacket> PlayerReadyPacket::deserialize(const std::vector<uint8_t> &data)
    {
        size_t length = data.size();
        const uint8_t *indexPtr = data.data();
        std::string username;
        PacketType type;

        type = Serializer::deserialize<PacketType>(indexPtr, length);
        if (type != PacketType::PLAYER_READY)
            throw InvalidPacketFormatException("PlayerReadyPacket should be of type PacketType::PLAYER_READY");
        username = Serializer::deserializeString(indexPtr, length);
        return std::make_unique<PlayerReadyPacket>(username);
    }

    const std::string &PlayerReadyPacket::getUsername() const
    {
        return m_username;
    }
} // namespace Network
