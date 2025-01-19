#include "Packet.hpp"
#include <SFML/Network/Packet.hpp>
#include <cstring>
#include "network/exceptions/NetworkExceptions.hpp"
#include "network/packets/BulletHitPacket.hpp"
#include "network/packets/BulletsUpdatePacket.hpp"
#include "network/packets/EnemiesUpdatePacket.hpp"
#include "network/packets/EnemyDeathPacket.hpp"
#include "network/packets/ErrorPacket.hpp"
#include "network/packets/GameOverPacket.hpp"
#include "network/packets/GameStartPacket.hpp"
#include "network/packets/PlayerConnectPacket.hpp"
#include "network/packets/PlayerDeathPacket.hpp"
#include "network/packets/PlayerDisconnectPacket.hpp"
#include "network/packets/PlayerMovePacket.hpp"
#include "network/packets/PlayerReadyPacket.hpp"
#include "network/packets/PlayerShootPacket.hpp"
#include "network/packets/PlayersUpdatePacket.hpp"
#include "network/packets/WaitingUpdatePacket.hpp"

namespace Network
{

    std::unique_ptr<Packet> Packet::deserialize(const std::vector<uint8_t> &data)
    {
        PacketType type;
        if (data.size() < sizeof(PacketType)) {
            throw IncompletePacketException("Packet size is too small");
        }

        std::memcpy(&type, data.data(), sizeof(PacketType));

        switch (type) {
            case PacketType::ERROR: return ErrorPacket::deserialize(data);
            case PacketType::NONE: throw InvalidPacketFormatException("NONE is not a valid packet"); break;
            case PacketType::GAME_START: return GameStartPacket::deserialize(data); break;
            case PacketType::GAME_OVER: return GameOverPacket::deserialize(data); break;
            case PacketType::PLAYER_CONNECT: return PlayerConnectPacket::deserialize(data); break;
            case PacketType::PLAYER_DISCONNECT: return PlayerDisconnectPacket::deserialize(data); break;
            case PacketType::PLAYER_READY: return PlayerReadyPacket::deserialize(data); break;
            case PacketType::PLAYER_MOVE: return PlayerMovePacket::deserialize(data); break;
            case PacketType::PLAYER_SHOOT: return PlayerShootPacket::deserialize(data); break;
            case PacketType::PLAYER_DEATH: return PlayerDeathPacket::deserialize(data); break;
            case PacketType::PLAYERS_UPDATE: return PlayersUpdatePacket::deserialize(data); break;
            case PacketType::ENEMIES_UPDATE: return EnemiesUpdatePacket::deserialize(data); break;
            case PacketType::BULLETS_UPDATE: return BulletsUpdatePacket::deserialize(data); break;
            case PacketType::BULLET_HIT: return BulletHitPacket::deserialize(data); break;
            case PacketType::ENEMY_DEATH: return EnemyDeathPacket::deserialize(data); break;
            case PacketType::WAITING_UPDATE: return WaitingUpdatePacket::deserialize(data); break;
            default:
                throw UnsupportedPacketVersionException(
                    "Packet version " + std::to_string(static_cast<uint>(type)) + "could not be deserialized");
                break;
        }
    }

    std::ostream &operator<<(std::ostream &os, Packet::PacketType type)
    {
        switch (type) {
            case Packet::PacketType::ERROR: os << "ERROR"; break;
            case Packet::PacketType::NONE: os << "NONE"; break;
            case Packet::PacketType::GAME_START: os << "GAME_START"; break;
            case Packet::PacketType::GAME_OVER: os << "GAME_OVER"; break;
            case Packet::PacketType::PLAYER_CONNECT: os << "PLAYER_CONNECT"; break;
            case Packet::PacketType::PLAYER_DISCONNECT: os << "PLAYER_DISCONNECT"; break;
            case Packet::PacketType::PLAYER_READY: os << "PLAYER_READY"; break;
            case Packet::PacketType::PLAYER_MOVE: os << "PLAYER_MOVE"; break;
            case Packet::PacketType::PLAYER_SHOOT: os << "PLAYER_SHOOT"; break;
            case Packet::PacketType::PLAYER_DEATH: os << "PLAYER_DEATH"; break;
            case Packet::PacketType::PLAYERS_UPDATE: os << "PLAYERS_UPDATE"; break;
            case Packet::PacketType::ENEMIES_UPDATE: os << "ENEMIES_UPDATE"; break;
            case Packet::PacketType::BULLETS_UPDATE: os << "BULLETS_UPDATE"; break;
            case Packet::PacketType::BULLET_HIT: os << "BULLET_HIT"; break;
            case Packet::PacketType::ENEMY_DEATH: os << "ENEMY_DEATH"; break;
            case Packet::PacketType::WAITING_UPDATE: break;
        }
        return os;
    }
} // namespace Network

// TODO:
// - Real Network Manager with tcp and udp (common)
// - Client class std::queue<std::unique_ptr<Packet>> receivedPackets (server)
// - class Foobar : public packet
