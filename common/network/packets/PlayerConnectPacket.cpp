#include "PlayerConnectPacket.hpp"
#include <cstring>
#include <memory>
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayerConnectPacket::PlayerConnectPacket(const std::string &username) : m_username(username)
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
        size_t dataIndex = sizeof(type);

        if (length == 0)
            throw InvalidPacketFormatException("PlayerConnectPacket should have a username");

        buffer.resize(sizeof(type) + sizeof(length) + length);
        std::memcpy(buffer.data(), &type, sizeof(type));
        std::memcpy(buffer.data() + dataIndex, &length, sizeof(length));
        dataIndex += sizeof(length);
        std::memcpy(buffer.data() + dataIndex, m_username.data(), length);
        return buffer;
    }

    std::unique_ptr<PlayerConnectPacket> PlayerConnectPacket::deserialize(const std::vector<uint8_t> &data)
    {
        size_t length;
        std::string username;
        size_t dataIndex = sizeof(PacketType);

        if (data.size() <= dataIndex)
            throw IncompletePacketException("PlayerConnectPacket contains no data");
        std::memcpy(&length, data.data() + dataIndex, sizeof(size_t));

        dataIndex += sizeof(size_t);

        if (data.size() != length + dataIndex)
            throw IncompletePacketException("PlayerConnectPacket username is wrongly sized");

        username.resize(length);
        std::memcpy(username.data(), data.data() + dataIndex, length);

        return std::make_unique<PlayerConnectPacket>(username);
    }

    const std::string &PlayerConnectPacket::getUsername() const
    {
        return m_username;
    }
} // namespace Network
