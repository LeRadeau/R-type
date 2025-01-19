#include "network/packets/WaitingUpdatePacket.hpp"
#include <SFML/System/Vector2.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for WaitingUpdatePacket
TEST_CASE("WaitingUpdatePacket constructor initializes correctly", "[WaitingUpdatePacket]")
{
    size_t n = 4;
    Network::WaitingUpdatePacket packet(n);

    REQUIRE(packet.getType() == Network::Packet::PacketType::WAITING_UPDATE);
    REQUIRE(packet.getClientTotal() == n);
}

TEST_CASE("WaitingUpdatePacket serialization works correctly", "[WaitingUpdatePacket]")
{
    size_t n = 4;
    Network::WaitingUpdatePacket packet(n);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::WaitingUpdatePacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::WAITING_UPDATE);
    REQUIRE(packet.getClientTotal() == n);
}
