#include "network/packets/PlayerDeathPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for PlayerDeathPacket
TEST_CASE("PlayerDeathPacket constructor initializes correctly", "[PlayerDeathPacket]")
{
    std::string username = "Player1";

    Network::PlayerDeathPacket packet(username);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_DEATH);
    REQUIRE(packet.getUsername() == username);
}

TEST_CASE("PlayerDeathPacket serialization works correctly", "[PlayerDeathPacket]")
{
    std::string username = "Player1";
    Network::PlayerDeathPacket packet(username);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::PlayerDeathPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_DEATH);
    REQUIRE(deserializedPacket->getUsername() == username);
}
