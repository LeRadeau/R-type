#include "network/packets/BulletHitPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for BulletHitPacket
TEST_CASE("BulletHitPacket constructor initializes correctly", "[BulletHitPacket]")
{
    std::string bulletId = "enemy_123412353454_1234";

    Network::BulletHitPacket packet(bulletId);

    REQUIRE(packet.getType() == Network::Packet::PacketType::BULLET_HIT);
    REQUIRE(packet.getBulletId() == bulletId);
}

TEST_CASE("BulletHitPacket serialization works correctly", "[BulletHitPacket]")
{
    std::string bulletId = "player_1234123412341234_4312";
    Network::BulletHitPacket packet(bulletId);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::BulletHitPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::BULLET_HIT);
    REQUIRE(deserializedPacket->getBulletId() == bulletId);
}
