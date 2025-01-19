#include "GameStartPacket.hpp"
#include "network/Serializer.hpp"

namespace Network
{

    GameStartPacket::PacketType GameStartPacket::getType() const
    {
        return PacketType::GAME_START;
    }

    std::vector<uint8_t> GameStartPacket::serialize() const
    {
        PacketType type = getType();
        std::vector<uint8_t> buffer;

        Serializer::serialize(buffer, type);
        return buffer;
    }

    std::unique_ptr<GameStartPacket> GameStartPacket::deserialize(const std::vector<uint8_t> &)
    {
        return std::make_unique<GameStartPacket>();
    }
} // namespace Network
