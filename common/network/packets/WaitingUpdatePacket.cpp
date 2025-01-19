#include "WaitingUpdatePacket.hpp"
#include <cstring>
#include "network/Serializer.hpp"
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

        Serializer::serialize<PacketType>(buffer, type);
        Serializer::serialize<size_t>(buffer, m_clientTotal);
        return buffer;
    }

    std::unique_ptr<WaitingUpdatePacket> WaitingUpdatePacket::deserialize(const std::vector<uint8_t> &data)
    {
        const uint8_t *indexPtr = data.data();
        size_t length = data.size();
        PacketType type;
        size_t clientTotal;

        type = Serializer::deserialize<PacketType>(indexPtr, length);
        if (type != PacketType::WAITING_UPDATE)
            throw InvalidPacketFormatException("WaitingUpdatePacket should be of type PacketType::WAITING_UPDATE");
        clientTotal = Serializer::deserialize<size_t>(indexPtr, length);

        return std::make_unique<WaitingUpdatePacket>(clientTotal);
    }

    size_t WaitingUpdatePacket::getClientTotal() const
    {
        return m_clientTotal;
    }
} // namespace Network
