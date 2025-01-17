
#include "EnemiesUpdatePacket.hpp"
#include <cstring>
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    EnemiesUpdatePacket::EnemiesUpdatePacket(const std::vector<EnemyData> &data) : m_data(data)
    {
    }

    EnemiesUpdatePacket::PacketType EnemiesUpdatePacket::getType() const
    {
        return PacketType::ENEMIES_UPDATE;
    }

    std::vector<uint8_t> EnemiesUpdatePacket::serialize() const
    {
        std::vector<uint8_t> buffer;
        PacketType type = getType();
        size_t dataIndex = sizeof(type);
        size_t length = m_data.size();

        buffer.resize(sizeof(type));
        std::memcpy(buffer.data(), &type, sizeof(type));
        for (size_t i = 0; i < length; i++) {
            size_t idLength = m_data[i].id.size();

            buffer.resize(buffer.size() + idLength + sizeof(idLength) + sizeof(m_data[i].x) + sizeof(m_data[i].y)
                + sizeof(m_data[i].health));

            std::memcpy(buffer.data() + dataIndex, &idLength, sizeof(idLength));
            dataIndex += sizeof(idLength);

            std::memcpy(buffer.data() + dataIndex, m_data[i].id.data(), idLength);
            dataIndex += idLength;

            std::memcpy(buffer.data() + dataIndex, &m_data[i].x, sizeof(m_data[i].x));
            dataIndex += sizeof(m_data[i].x);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].y, sizeof(m_data[i].y));
            dataIndex += sizeof(m_data[i].y);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].health, sizeof(m_data[i].health));
            dataIndex += sizeof(m_data[i].health);
        }
        return buffer;
    }

    std::unique_ptr<EnemiesUpdatePacket> EnemiesUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        std::vector<EnemyData> buffer;
        size_t dataIndex = sizeof(type);
        size_t vectorLength;

        if (data.size() <= dataIndex)
            throw IncompletePacketException("EnemiesUpdatePacket contains no data");
        std::memcpy(&type, data.data(), sizeof(type));

        if (data.size() <= dataIndex + sizeof(vectorLength))
            throw IncompletePacketException("EnemiesUpdatePacket contains no data after size");

        std::memcpy(&vectorLength, data.data() + dataIndex, sizeof(vectorLength));
        dataIndex += sizeof(vectorLength);
        for (size_t i = 0; i < vectorLength; i++) {
            size_t usernameLength;
            EnemyData enemy;

            if (data.size() <= dataIndex + sizeof(usernameLength))
                throw IncompletePacketException("EnemiesUpdatePacket ended on id length");
            std::memcpy(&usernameLength, data.data() + dataIndex, sizeof(usernameLength));
            dataIndex += sizeof(usernameLength);

            if (data.size() <= dataIndex + usernameLength)
                throw IncompletePacketException("EnemiesUpdatePacket ended on id");
            enemy.id.resize(usernameLength);
            std::memcpy(enemy.id.data(), data.data() + dataIndex, usernameLength);
            dataIndex += usernameLength;

            if (data.size() <= dataIndex + sizeof(enemy.x))
                throw IncompletePacketException("EnemiesUpdatePacket ended on position x");
            std::memcpy(&enemy.x, data.data() + dataIndex, sizeof(enemy.x));
            dataIndex += sizeof(enemy.x);

            if (data.size() <= dataIndex + sizeof(enemy.y))
                throw IncompletePacketException("EnemiesUpdatePacket ended on position y");
            std::memcpy(&enemy.y, data.data() + dataIndex, sizeof(enemy.y));
            dataIndex += sizeof(enemy.y);

            if (data.size() < dataIndex + sizeof(enemy.health))
                throw IncompletePacketException("EnemiesUpdatePacket ended on health");
            std::memcpy(&enemy.health, data.data() + dataIndex, sizeof(enemy.health));
            dataIndex += sizeof(enemy.health);
        }
        return std::make_unique<EnemiesUpdatePacket>(buffer);
    }

    const std::vector<EnemiesUpdatePacket::EnemyData> &EnemiesUpdatePacket::getData() const
    {
        return m_data;
    }
} // namespace Network
