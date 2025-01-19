#include "network/packets/EnemyDeathPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for EnemyDeathPacket
TEST_CASE("EnemyDeathPacket constructor initializes correctly", "[EnemyDeathPacket]")
{
    std::string bulletId = "enemy_123412353454_1234";

    Network::EnemyDeathPacket packet(bulletId);

    REQUIRE(packet.getType() == Network::Packet::PacketType::ENEMY_DEATH);
    REQUIRE(packet.getEnemyId() == bulletId);
}

TEST_CASE("EnemyDeathPacket serialization works correctly", "[EnemyDeathPacket]")
{
    std::string bulletId = "player_1234123412341234_4312";
    Network::EnemyDeathPacket packet(bulletId);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::EnemyDeathPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::ENEMY_DEATH);
    REQUIRE(deserializedPacket->getEnemyId() == bulletId);
}
