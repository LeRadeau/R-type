#include "ErrorPacket.hpp"
#include <cstring>
#include <memory>
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
        buffer.resize(sizeof(type) + sizeof(length) + length);
        std::memcpy(buffer.data(), &type, sizeof(type));
        std::memcpy(buffer.data(), &length, sizeof(length));
        std::memcpy(buffer.data() + sizeof(length), m_errorMessage.data(), length);
        return buffer;
    }

    std::unique_ptr<ErrorPacket> ErrorPacket::deserialize(const std::vector<uint8_t> &data)
    {
        size_t length;
        std::string message;
        size_t dataIndex = sizeof(PacketType);

        if (data.size() <= dataIndex)
            throw IncompletePacketException("ErrorPacket contains no data");
        std::memcpy(&length, data.data() + dataIndex, sizeof(size_t));

        dataIndex += sizeof(size_t);

        if (data.size() != length + dataIndex)
            throw IncompletePacketException("ErrorPacket message is wrongly sized");

        message.resize(length);
        std::memcpy(message.data(), data.data() + dataIndex, length);

        return std::make_unique<ErrorPacket>(message);
    }

    const std::string &ErrorPacket::getErrorMessage() const
    {
        return m_errorMessage;
    }
} // namespace Network
