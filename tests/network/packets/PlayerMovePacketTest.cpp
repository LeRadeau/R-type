
#include "network/packets/PlayerMovePacket.hpp"
#include <SFML/System/Vector2.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for PlayerMovePacket
TEST_CASE("PlayerMovePacket constructor initializes correctly", "[PlayerMovePacket]")
{
    std::string username = "Player1";
    sf::Vector2f position(420, 1337);

    Network::PlayerMovePacket packet(username, position.x, position.y);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_MOVE);
    REQUIRE(packet.getUsername() == username);
    REQUIRE(packet.getX() == position.x);
    REQUIRE(packet.getY() == position.y);
}

TEST_CASE("PlayerMovePacket serialization works correctly", "[PlayerMovePacket]")
{
    std::string username = "Player1";
    sf::Vector2f position(420, 1337);
    Network::PlayerMovePacket packet(username, position.x, position.y);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::PlayerMovePacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYER_MOVE);
    REQUIRE(packet.getUsername() == username);
    REQUIRE(packet.getX() == position.x);
    REQUIRE(packet.getY() == position.y);
}
