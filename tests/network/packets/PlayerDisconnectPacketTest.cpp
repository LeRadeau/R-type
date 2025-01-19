#include "network/packets/PlayerDisconnectPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for PlayerDisconnectPacket
TEST_CASE("PlayerDisconnectPacket constructor initializes correctly", "[PlayerDisconnectPacket]")
{
    std::string username = "Player1";

    Network::PlayerDisconnectPacket packet(username);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_DISCONNECT);
    REQUIRE(packet.getUsername() == username);
}

TEST_CASE("PlayerDisconnectPacket serialization works correctly", "[PlayerDisconnectPacket]")
{
    std::string username = "Player1";
    Network::PlayerDisconnectPacket packet(username);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::PlayerDisconnectPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_DISCONNECT);
    REQUIRE(deserializedPacket->getUsername() == username);
}
