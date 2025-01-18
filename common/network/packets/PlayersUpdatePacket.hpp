#pragma once

#include "Packet.hpp"

namespace Network
{
    class PlayersUpdatePacket : public Packet {
      public:
        struct ClientData {
            std::string username;
            float x;
            float y;
            int health;
            int score;
        };

        explicit PlayersUpdatePacket(const std::vector<ClientData> &data);
        PacketType getType() const override;
        std::vector<uint8_t> serialize() const override;
        static std::unique_ptr<PlayersUpdatePacket> deserialize(const std::vector<uint8_t> &data);

        const std::vector<ClientData> &getData() const;

      private:
        std::vector<ClientData> m_data;
    };
} // namespace Network
