#include "PlayerDisconnectPacket.hpp"
#include <cstring>
#include <memory>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayerDisconnectPacket::PlayerDisconnectPacket(const std::string &username) : m_username(username)
    {
    }

    PlayerDisconnectPacket::PacketType PlayerDisconnectPacket::getType() const
    {
        return PacketType::PLAYER_DISCONNECT;
    }
    std::vector<uint8_t> PlayerDisconnectPacket::serialize() const
    {
        std::vector<uint8_t> buffer;
        PacketType type = getType();
        size_t length = m_username.size();

        if (length == 0)
            throw InvalidPacketFormatException("PlayerDisconnectPacket should have a username");
        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, m_username);
        return buffer;
    }

    std::unique_ptr<PlayerDisconnectPacket> PlayerDisconnectPacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string username;
        PacketType type;
        size_t size = data.size();
        const uint8_t *indexPtr = data.data();

        type = Serializer::deserialize<PacketType>(indexPtr, size);
        if (type != PacketType::PLAYER_DISCONNECT)
            throw InvalidPacketFormatException(
                "PlayerDisconnectPacket should be of type PacketType::PLAYER_DISCONNECT");
        username = Serializer::deserializeString(indexPtr, size);
        return std::make_unique<PlayerDisconnectPacket>(username);
    }

    const std::string &PlayerDisconnectPacket::getUsername() const
    {
        return m_username;
    }
} // namespace Network
