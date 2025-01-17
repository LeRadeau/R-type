#include "WaitingUpdatePacket.hpp"
#include <cstring>
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{

    WaitingUpdatePacket::WaitingUpdatePacket(size_t clientTotal) : m_clientTotal(clientTotal)
    {
    }

    Packet::PacketType WaitingUpdatePacket::getType() const
    {
        return PacketType::WAITING_UPDATE;
    }

    std::vector<uint8_t> WaitingUpdatePacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t dataIndex = sizeof(type);

        buffer.resize(sizeof(type) + sizeof(m_clientTotal));
        std::memcpy(buffer.data(), &type, sizeof(type));
        std::memcpy(buffer.data() + dataIndex, &m_clientTotal, sizeof(m_clientTotal));
        return buffer;
    }

    std::unique_ptr<WaitingUpdatePacket> WaitingUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        size_t clientTotal;
        size_t dataIndex = sizeof(type);

        if (data.size() < sizeof(type))
            throw InvalidPacketFormatException("WaitingUpdatePacket contains no data");
        std::memcpy(&type, data.data(), sizeof(type));

        if (data.size() < dataIndex + sizeof(clientTotal))
            throw InvalidPacketFormatException("WaitingUpdatePacket is missing clientTotal");
        std::memcpy(&clientTotal, data.data() + dataIndex, sizeof(clientTotal));

        return std::make_unique<WaitingUpdatePacket>(clientTotal);
    }

    size_t WaitingUpdatePacket::getClientTotal() const
    {
        return m_clientTotal;
    }
} // namespace Network
