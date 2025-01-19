#include "PlayerDeathPacket.hpp"
#include <cstring>
#include <memory>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayerDeathPacket::PlayerDeathPacket(const std::string &username) : m_username(username)
    {
    }

    PlayerDeathPacket::PacketType PlayerDeathPacket::getType() const
    {
        return PacketType::PLAYER_DEATH;
    }
    std::vector<uint8_t> PlayerDeathPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t length = m_username.size();

        if (length == 0)
            throw InvalidPacketFormatException("PlayerDeathPacket should have a username");
        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, m_username);
        return buffer;
    }

    std::unique_ptr<PlayerDeathPacket> PlayerDeathPacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string username;
        size_t size = data.size();
        const uint8_t *indexPtr = data.data();
        PacketType type;

        type = Serializer::deserialize<PacketType>(indexPtr, size);
        if (type != PacketType::PLAYER_DEATH)
            throw InvalidPacketFormatException("PlayerDeathPacket should be of type PacketType::PLAYER_DEATH");
        username = Serializer::deserializeString(indexPtr, size);
        return std::make_unique<PlayerDeathPacket>(username);
    }

    const std::string &PlayerDeathPacket::getUsername() const
    {
        return m_username;
    }
} // namespace Network
