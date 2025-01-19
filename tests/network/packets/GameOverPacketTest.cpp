#include "network/packets/GameOverPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for GameOverPacket
TEST_CASE("GameOverPacket constructor initializes correctly", "[GameOverPacket]")
{
    Network::GameOverPacket packet;

    REQUIRE(packet.getType() == Network::Packet::PacketType::GAME_OVER);
}

TEST_CASE("GameOverPacket serialization works correctly", "[GameOverPacket]")
{
    Network::GameOverPacket packet;

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::GameOverPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::GAME_OVER);
}
