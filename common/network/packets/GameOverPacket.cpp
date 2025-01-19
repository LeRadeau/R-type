#include "GameOverPacket.hpp"
#include "network/Serializer.hpp"

namespace Network
{
    GameOverPacket::PacketType GameOverPacket::getType() const
    {
        return PacketType::GAME_OVER;
    }

    std::vector<uint8_t> GameOverPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;

        Serializer::serialize(buffer, type);
        return buffer;
    }

    std::unique_ptr<GameOverPacket> GameOverPacket::deserialize(const std::vector<uint8_t> &)
    {
        return std::make_unique<GameOverPacket>();
    }
} // namespace Network
