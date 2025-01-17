#include "PlayersUpdatePacket.hpp"
#include <cstring>
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
        size_t dataIndex = sizeof(type);
        size_t length = m_data.size();

        buffer.resize(sizeof(type));
        std::memcpy(buffer.data(), &type, sizeof(type));
        for (size_t i = 0; i < length; i++) {
            size_t usernameLength = m_data[i].username.size();

            buffer.resize(buffer.size() + usernameLength + sizeof(usernameLength) + sizeof(m_data[i].x)
                + sizeof(m_data[i].y) + sizeof(m_data[i].health) + sizeof(m_data[i].score));

            std::memcpy(buffer.data() + dataIndex, &usernameLength, sizeof(usernameLength));
            dataIndex += sizeof(usernameLength);

            std::memcpy(buffer.data() + dataIndex, m_data[i].username.data(), usernameLength);
            dataIndex += usernameLength;

            std::memcpy(buffer.data() + dataIndex, &m_data[i].x, sizeof(m_data[i].x));
            dataIndex += sizeof(m_data[i].x);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].y, sizeof(m_data[i].y));
            dataIndex += sizeof(m_data[i].y);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].health, sizeof(m_data[i].health));
            dataIndex += sizeof(m_data[i].health);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].score, sizeof(m_data[i].score));
            dataIndex += sizeof(m_data[i].score);
        }
        return buffer;
    }

    std::unique_ptr<PlayersUpdatePacket> PlayersUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        std::vector<ClientData> buffer;
        size_t dataIndex = sizeof(type);
        size_t vectorLength;

        if (data.size() <= dataIndex)
            throw IncompletePacketException("PlayersUpdatePacket contains no data");
        std::memcpy(&type, data.data(), sizeof(type));

        if (data.size() <= dataIndex + sizeof(vectorLength))
            throw IncompletePacketException("PlayersUpdatePacket contains no data after size");

        std::memcpy(&vectorLength, data.data() + dataIndex, sizeof(vectorLength));
        dataIndex += sizeof(vectorLength);
        for (size_t i = 0; i < vectorLength; i++) {
            size_t usernameLength;
            ClientData client;

            if (data.size() <= dataIndex + sizeof(usernameLength))
                throw IncompletePacketException("PlayersUpdatePacket ended on username length");
            std::memcpy(&usernameLength, data.data() + dataIndex, sizeof(usernameLength));
            dataIndex += sizeof(usernameLength);

            if (data.size() <= dataIndex + usernameLength)
                throw IncompletePacketException("PlayersUpdatePacket ended on username");
            client.username.resize(usernameLength);
            std::memcpy(client.username.data(), data.data() + dataIndex, usernameLength);
            dataIndex += usernameLength;

            if (data.size() <= dataIndex + sizeof(client.x))
                throw IncompletePacketException("PlayersUpdatePacket ended on position x");
            std::memcpy(&client.x, data.data() + dataIndex, sizeof(client.x));
            dataIndex += sizeof(client.x);

            if (data.size() <= dataIndex + sizeof(client.y))
                throw IncompletePacketException("PlayersUpdatePacket ended on position y");
            std::memcpy(&client.y, data.data() + dataIndex, sizeof(client.y));
            dataIndex += sizeof(client.y);

            if (data.size() <= dataIndex + sizeof(client.health))
                throw IncompletePacketException("PlayersUpdatePacket ended on health");
            std::memcpy(&client.health, data.data() + dataIndex, sizeof(client.health));
            dataIndex += sizeof(client.health);

            if (data.size() < dataIndex + sizeof(client.score))
                throw IncompletePacketException("PlayersUpdatePacket ended on score");
            std::memcpy(&client.score, data.data() + dataIndex, sizeof(client.score));
        }
        return std::make_unique<PlayersUpdatePacket>(buffer);
    }

    const std::vector<PlayersUpdatePacket::ClientData> &PlayersUpdatePacket::getData() const
    {
        return m_data;
    }
} // namespace Network
