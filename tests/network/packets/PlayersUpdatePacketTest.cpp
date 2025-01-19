#include "network/packets/PlayersUpdatePacket.hpp"
#include <SFML/System/Vector2.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for PlayersUpdatePacket
TEST_CASE("PlayersUpdatePacket constructor initializes correctly", "[PlayersUpdatePacket]")
{
    std::vector<Network::PlayersUpdatePacket::ClientData> data;
    std::string id = "BarFoo";
    float x = 123;
    float y = 634;
    int health = 100;
    int score = 1337;
    size_t size = 10;

    Network::PlayersUpdatePacket::ClientData enemyData({id, x, y, health, score});
    for (size_t i = 0; i < size; i++) {
        id.push_back('F');
        enemyData.x += 1;
        enemyData.y += 1;
        enemyData.health -= 1;
        enemyData.score -= 1;
        data.push_back(enemyData);
    }
    Network::PlayersUpdatePacket packet(data);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYERS_UPDATE);
    auto packetData = packet.getData();
    REQUIRE(packetData.size() == data.size());
    for (auto i = 0; i < data.size(); i++) {
        REQUIRE(packetData[i].username == data[i].username);
        REQUIRE(packetData[i].x == data[i].x);
        REQUIRE(packetData[i].y == data[i].y);
        REQUIRE(packetData[i].health == data[i].health);
        REQUIRE(packetData[i].score == data[i].score);
    }
}

TEST_CASE("PlayersUpdatePacket serialization works correctly", "[PlayersUpdatePacket]")
{
    std::vector<Network::PlayersUpdatePacket::ClientData> data;
    std::string id = "FooBar";
    float x = 123;
    float y = 634;
    int health = 100;
    int score = 1337;
    size_t size = 10;

    Network::PlayersUpdatePacket::ClientData enemyData({id, x, y, health, score});
    for (size_t i = 0; i < size; i++) {
        id.push_back('F');
        enemyData.x += 1;
        enemyData.y += 1;
        enemyData.health -= 1;
        enemyData.score -= 1;
        data.push_back(enemyData);
    }
    Network::PlayersUpdatePacket packet(data);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::PlayersUpdatePacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::PLAYERS_UPDATE);
    auto packetData = deserializedPacket->getData();
    REQUIRE(packetData.size() == data.size());
    for (auto i = 0; i < data.size(); i++) {
        REQUIRE(packetData[i].username == data[i].username);
        REQUIRE(packetData[i].x == data[i].x);
        REQUIRE(packetData[i].y == data[i].y);
        REQUIRE(packetData[i].health == data[i].health);
        REQUIRE(packetData[i].score == data[i].score);
    }
}
