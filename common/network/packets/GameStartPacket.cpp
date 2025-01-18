#include "GameStartPacket.hpp"
#include <cstring>
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

        buffer.resize(sizeof(type));
        std::memcpy(buffer.data(), &type, sizeof(type));
        return buffer;
    }

    std::unique_ptr<GameStartPacket> GameStartPacket::deserialize(const std::vector<uint8_t> &)
    {
        return std::make_unique<GameStartPacket>();
    }
} // namespace Network
