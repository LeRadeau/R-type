#include "network/packets/PlayerReadyPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for PlayerReadyPacket
TEST_CASE("PlayerReadyPacket constructor initializes correctly", "[PlayerReadyPacket]")
{
    std::string username = "UUUU";
    Network::PlayerReadyPacket packet(username);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_READY);
    REQUIRE(packet.getUsername() == username);
}

TEST_CASE("PlayerReadyPacket serialization works correctly", "[PlayerReadyPacket]")
{
    std::string username = "UUUU";
    Network::PlayerReadyPacket packet(username);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::PlayerReadyPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_READY);
    REQUIRE(packet.getUsername() == username);
}
