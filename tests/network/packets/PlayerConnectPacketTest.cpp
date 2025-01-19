#include "network/packets/PlayerConnectPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for PlayerConnectPacket
TEST_CASE("PlayerConnectPacket constructor initializes correctly", "[PlayerConnectPacket]")
{
    std::string username = "Player1";
    unsigned short udpPort = 12345;

    Network::PlayerConnectPacket packet(username, udpPort);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_CONNECT);
    REQUIRE(packet.getUsername() == username);
    REQUIRE(packet.getPort() == udpPort);
}

TEST_CASE("PlayerConnectPacket serialization works correctly", "[PlayerConnectPacket]")
{
    std::string username = "Player1";
    unsigned short udpPort = 12345;
    Network::PlayerConnectPacket packet(username, udpPort);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::PlayerConnectPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_CONNECT);
    REQUIRE(deserializedPacket->getUsername() == username);
    REQUIRE(deserializedPacket->getPort() == udpPort);
}
