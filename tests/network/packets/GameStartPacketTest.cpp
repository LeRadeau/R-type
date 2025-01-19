#include "network/packets/GameStartPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for GameStartPacket
TEST_CASE("GameStartPacket constructor initializes correctly", "[GameStartPacket]")
{
    Network::GameStartPacket packet;

    REQUIRE(packet.getType() == Network::Packet::PacketType::GAME_START);
}

TEST_CASE("GameStartPacket serialization works correctly", "[GameStartPacket]")
{
    Network::GameStartPacket packet;

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::GameStartPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::GAME_START);
}
