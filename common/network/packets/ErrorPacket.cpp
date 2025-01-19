#include "ErrorPacket.hpp"
#include <memory>
#include <string>
#include "network/Serializer.hpp"
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    ErrorPacket::ErrorPacket(const std::string &errorMessage) : m_errorMessage(errorMessage)
    {
    }

    ErrorPacket::PacketType ErrorPacket::getType() const
    {
        return PacketType::ERROR;
    }

    std::vector<uint8_t> ErrorPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;
        size_t length = m_errorMessage.size();
        if (length == 0)
            throw InvalidPacketFormatException("ErrorPacket should have a message defined");
        Serializer::serialize<PacketType>(buffer, type);
        Serializer::serialize(buffer, m_errorMessage);
        return buffer;
    }

    std::unique_ptr<ErrorPacket> ErrorPacket::deserialize(const std::vector<uint8_t> &data)
    {
        std::string message;
        PacketType type;
        const uint8_t *indexPtr = data.data();
        size_t size = data.size();

        type = Serializer::deserialize<PacketType>(indexPtr, size);
        if (type != PacketType::ERROR)
            throw Network::InvalidPacketFormatException("ErrorPacket should be of type PacketType::ERROR");
        message = Serializer::deserializeString(indexPtr, size);
        return std::make_unique<ErrorPacket>(message);
    }

    const std::string &ErrorPacket::getErrorMessage() const
    {
        return m_errorMessage;
    }
} // namespace Network
