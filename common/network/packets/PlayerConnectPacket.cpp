#include "PlayerConnectPacket.hpp"
#include <cstring>
#include <memory>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayerConnectPacket::PlayerConnectPacket(const std::string &username, unsigned short udpPort)
        : m_username(username), m_udpPort(udpPort)
    {
    }

    PlayerConnectPacket::PacketType PlayerConnectPacket::getType() const
    {
        return PacketType::PLAYER_CONNECT;
    }
    std::vector<uint8_t> PlayerConnectPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t length = m_username.size();

        if (length == 0)
            throw InvalidPacketFormatException("PlayerConnectPacket should have a username");
        if (m_udpPort == 0)
            throw InvalidPacketFormatException("PlayerConnectPacket should have a udpPort different of zero");

        Serializer::serialize(buffer, type);
        Serializer::serialize(buffer, m_username);
        Serializer::serialize(buffer, m_udpPort);
        return buffer;
    }

    std::unique_ptr<PlayerConnectPacket> PlayerConnectPacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string username;
        unsigned short udpPort;
        size_t size = data.size();
        const uint8_t *indexPtr = data.data();
        PacketType type;

        type = Serializer::deserialize<PacketType>(indexPtr, size);
        if (type != PacketType::PLAYER_CONNECT)
            throw InvalidPacketFormatException("PlayerConnectPacket should be of type PacketType::PLAYER_CONNECT");
        username = Serializer::deserializeString(indexPtr, size);
        udpPort = Serializer::deserialize<unsigned short>(indexPtr, size);
        return std::make_unique<PlayerConnectPacket>(username, udpPort);
    }

    const std::string &PlayerConnectPacket::getUsername() const
    {
        return m_username;
    }

    unsigned short PlayerConnectPacket::getPort() const
    {
        return m_udpPort;
    }
} // namespace Network
