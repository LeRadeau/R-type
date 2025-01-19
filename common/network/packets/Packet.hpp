#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace Network
{
    class Packet {
      public:
        virtual ~Packet() = default;
        enum class PacketType {
            ERROR = -1,
            NONE = 0,
            GAME_START,
            GAME_OVER,
            PLAYER_CONNECT,
            PLAYER_DISCONNECT,
            PLAYER_READY,
            PLAYER_MOVE,
            PLAYER_SHOOT,
            PLAYER_DEATH,
            PLAYERS_UPDATE,
            ENEMIES_UPDATE,
            BULLETS_UPDATE,
            BULLET_HIT,
            ENEMY_DEATH,
            WAITING_UPDATE,
        };
        virtual PacketType getType() const = 0;
        virtual std::vector<uint8_t> serialize() const = 0;
        static std::unique_ptr<Packet> deserialize(const std::vector<uint8_t> &);
    };
    std::ostream &operator<<(std::ostream &os, Packet::PacketType type);
} // namespace Network
