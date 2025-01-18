#include "BulletsUpdatePacket.hpp"
#include <cstring>
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    BulletsUpdatePacket::BulletsUpdatePacket(const std::vector<BulletData> &data) : m_data(data)
    {
    }

    BulletsUpdatePacket::PacketType BulletsUpdatePacket::getType() const
    {
        return PacketType::BULLETS_UPDATE;
    }

    std::vector<uint8_t> BulletsUpdatePacket::serialize() const
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
                + sizeof(m_data[i].vx) + sizeof(m_data[i].vy));

            std::memcpy(buffer.data() + dataIndex, &idLength, sizeof(idLength));
            dataIndex += sizeof(idLength);

            std::memcpy(buffer.data() + dataIndex, m_data[i].id.data(), idLength);
            dataIndex += idLength;

            std::memcpy(buffer.data() + dataIndex, &m_data[i].x, sizeof(m_data[i].x));
            dataIndex += sizeof(m_data[i].x);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].y, sizeof(m_data[i].y));
            dataIndex += sizeof(m_data[i].y);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].vx, sizeof(m_data[i].vx));
            dataIndex += sizeof(m_data[i].vx);

            std::memcpy(buffer.data() + dataIndex, &m_data[i].vy, sizeof(m_data[i].vy));
            dataIndex += sizeof(m_data[i].vy);
        }
        return buffer;
    }

    std::unique_ptr<BulletsUpdatePacket> BulletsUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        std::vector<BulletData> buffer;
        size_t dataIndex = sizeof(type);
        size_t vectorLength;

        if (data.size() <= dataIndex)
            throw IncompletePacketException("BulletsUpdatePacket contains no data");
        std::memcpy(&type, data.data(), sizeof(type));

        if (data.size() <= dataIndex + sizeof(vectorLength))
            throw IncompletePacketException("BulletsUpdatePacket contains no data after size");

        std::memcpy(&vectorLength, data.data() + dataIndex, sizeof(vectorLength));
        dataIndex += sizeof(vectorLength);
        for (size_t i = 0; i < vectorLength; i++) {
            size_t usernameLength;
            BulletData enemy;

            if (data.size() <= dataIndex + sizeof(usernameLength))
                throw IncompletePacketException("BulletsUpdatePacket ended on id length");
            std::memcpy(&usernameLength, data.data() + dataIndex, sizeof(usernameLength));
            dataIndex += sizeof(usernameLength);

            if (data.size() <= dataIndex + usernameLength)
                throw IncompletePacketException("BulletsUpdatePacket ended on id");
            enemy.id.resize(usernameLength);
            std::memcpy(enemy.id.data(), data.data() + dataIndex, usernameLength);
            dataIndex += usernameLength;

            if (data.size() <= dataIndex + sizeof(enemy.x))
                throw IncompletePacketException("BulletsUpdatePacket ended on position x");
            std::memcpy(&enemy.x, data.data() + dataIndex, sizeof(enemy.x));
            dataIndex += sizeof(enemy.x);

            if (data.size() <= dataIndex + sizeof(enemy.y))
                throw IncompletePacketException("BulletsUpdatePacket ended on position y");
            std::memcpy(&enemy.y, data.data() + dataIndex, sizeof(enemy.y));
            dataIndex += sizeof(enemy.y);

            if (data.size() <= dataIndex + sizeof(enemy.vx))
                throw IncompletePacketException("BulletsUpdatePacket ended on vx");
            std::memcpy(&enemy.vx, data.data() + dataIndex, sizeof(enemy.vx));
            dataIndex += sizeof(enemy.vx);

            if (data.size() < dataIndex + sizeof(enemy.vy))
                throw IncompletePacketException("BulletsUpdatePacket ended on vy");
            std::memcpy(&enemy.vy, data.data() + dataIndex, sizeof(enemy.vy));
            dataIndex += sizeof(enemy.vy);
        }
        return std::make_unique<BulletsUpdatePacket>(buffer);
    }

    const std::vector<BulletsUpdatePacket::BulletData> &BulletsUpdatePacket::getData() const
    {
        return m_data;
    }

} // namespace Network
