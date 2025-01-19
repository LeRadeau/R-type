#include "network/packets/ErrorPacket.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for ErrorPacket
TEST_CASE("ErrorPacket constructor initializes correctly", "[ErrorPacket]")
{
    std::string message = "Server failed miserably";

    Network::ErrorPacket packet(message);

    REQUIRE(packet.getType() == Network::Packet::PacketType::ERROR);
    REQUIRE(packet.getErrorMessage() == message);
}

TEST_CASE("ErrorPacket serialization works correctly", "[ErrorPacket]")
{
    std::string message = "Client is bad";
    Network::ErrorPacket packet(message);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::ErrorPacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::ERROR);
    REQUIRE(deserializedPacket->getErrorMessage() == message);
}
