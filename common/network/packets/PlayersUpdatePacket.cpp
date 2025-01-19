#include "PlayersUpdatePacket.hpp"
#include <cstring>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    PlayersUpdatePacket::PlayersUpdatePacket(const std::vector<ClientData> &data) : m_data(data)
    {
    }

    PlayersUpdatePacket::PacketType PlayersUpdatePacket::getType() const
    {
        return PacketType::PLAYERS_UPDATE;
    }

    std::vector<uint8_t> PlayersUpdatePacket::serialize() const
    {
        std::vector<uint8_t> buffer;
        PacketType type = getType();
        size_t length = m_data.size();

        Serializer::serialize<PacketType>(buffer, type);
        Serializer::serialize<size_t>(buffer, length);
        for (size_t i = 0; i < length; i++) {
            Serializer::serialize(buffer, m_data[i].username);
            Serializer::serialize(buffer, m_data[i].x);
            Serializer::serialize(buffer, m_data[i].y);
            Serializer::serialize(buffer, m_data[i].health);
            Serializer::serialize(buffer, m_data[i].score);
        }
        return buffer;
    }

    std::unique_ptr<PlayersUpdatePacket> PlayersUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        std::vector<ClientData> buffer;
        size_t vectorLength;
        size_t length = data.size();
        const uint8_t *indexPtr = data.data();

        type = Serializer::deserialize<PacketType>(indexPtr, length);
        if (type != PacketType::PLAYERS_UPDATE)
            throw InvalidPacketFormatException("PlayersUpdatePacket should be of type PacketType::PLAYERS_UPDATE");
        vectorLength = Serializer::deserialize<size_t>(indexPtr, length);
        for (size_t i = 0; i < vectorLength; i++) {
            ClientData client;
            client.username = Serializer::deserializeString(indexPtr, length);
            client.x = Serializer::deserialize<float>(indexPtr, length);
            client.y = Serializer::deserialize<float>(indexPtr, length);
            client.health = Serializer::deserialize<int>(indexPtr, length);
            client.score = Serializer::deserialize<int>(indexPtr, length);
            buffer.push_back(client);
        }
        return std::make_unique<PlayersUpdatePacket>(buffer);
    }

    const std::vector<PlayersUpdatePacket::ClientData> &PlayersUpdatePacket::getData() const
    {
        return m_data;
    }
} // namespace Network
