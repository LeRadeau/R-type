#include "network/packets/Packet.hpp"
#include "network/exceptions/NetworkExceptions.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Packet deserialization handles errors", "[Packet]")
{
    std::vector<uint8_t> malformedData = {0x00};
    std::vector<uint8_t> emptyData;

    REQUIRE_THROWS_AS(Network::Packet::deserialize(malformedData), Network::IncompletePacketException);
    REQUIRE_THROWS_AS(Network::Packet::deserialize(emptyData), Network::IncompletePacketException);
}
