#pragma once
#include "Packet.hpp"

namespace Network
{
    class ErrorPacket : public Packet {
      public:
        explicit ErrorPacket(const std::string &errorMessage);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<ErrorPacket> deserialize(const std::vector<uint8_t> &data);

        const std::string &getErrorMessage() const;

      private:
        std::string m_errorMessage;
    };
} // namespace Network
