#include "network/packets/PlayerShootPacket.hpp"
#include <SFML/System/Vector2.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for PlayerShootPacket
TEST_CASE("PlayerShootPacket constructor initializes correctly", "[PlayerShootPacket]")
{
    std::string username = "UUUU";
    sf::Vector2f position(420, 1337);

    Network::PlayerShootPacket packet(username, position.x, position.y);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_SHOOT);
    REQUIRE(packet.getUsername() == username);
    REQUIRE(packet.getX() == position.x);
    REQUIRE(packet.getY() == position.y);
}

TEST_CASE("PlayerShootPacket serialization works correctly", "[PlayerShootPacket]")
{
    std::string username = "UUUU";
    sf::Vector2f position(420, 1337);
    Network::PlayerShootPacket packet(username, position.x, position.y);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::PlayerShootPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_SHOOT);
    REQUIRE(packet.getUsername() == username);
    REQUIRE(packet.getX() == position.x);
    REQUIRE(packet.getY() == position.y);
}
