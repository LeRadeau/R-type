#include "network/packets/EnemiesUpdatePacket.hpp"
#include <SFML/System/Vector2.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for EnemiesUpdatePacket
TEST_CASE("EnemiesUpdatePacket constructor initializes correctly", "[EnemiesUpdatePacket]")
{
    std::vector<Network::EnemiesUpdatePacket::EnemyData> data;
    std::string id = "enemy_1234123415_1412";
    float x = 123;
    float y = 634;
    int health = 100;
    size_t size = 10;

    Network::EnemiesUpdatePacket::EnemyData enemyData({id, x, y, health});
    for (size_t i = 0; i < size; i++) {
        id.push_back('F');
        enemyData.x += 1;
        enemyData.y += 1;
        enemyData.health -= 1;
        data.push_back(enemyData);
    }
    Network::EnemiesUpdatePacket packet(data);

    REQUIRE(packet.getType() == Network::Packet::PacketType::ENEMIES_UPDATE);
    auto packetData = packet.getData();
    REQUIRE(packetData.size() == data.size());
    for (auto i = 0; i < data.size(); i++) {
        REQUIRE(packetData[i].id == data[i].id);
        REQUIRE(packetData[i].x == data[i].x);
        REQUIRE(packetData[i].y == data[i].y);
        REQUIRE(packetData[i].health == data[i].health);
    }
}

TEST_CASE("EnemiesUpdatePacket serialization works correctly", "[EnemiesUpdatePacket]")
{
    std::vector<Network::EnemiesUpdatePacket::EnemyData> data;
    std::string id = "enemy_1234123415_1412";
    float x = 123;
    float y = 634;
    int health = 100;
    size_t size = 10;

    Network::EnemiesUpdatePacket::EnemyData enemyData({id, x, y, health});
    for (size_t i = 0; i < size; i++) {
        id.push_back('F');
        enemyData.x += 1;
        enemyData.y += 1;
        enemyData.health -= 1;
        data.push_back(enemyData);
    }
    Network::EnemiesUpdatePacket packet(data);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::EnemiesUpdatePacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::ENEMIES_UPDATE);
    auto packetData = deserializedPacket->getData();
    REQUIRE(packetData.size() == data.size());
    for (auto i = 0; i < data.size(); i++) {
        REQUIRE(packetData[i].id == data[i].id);
        REQUIRE(packetData[i].x == data[i].x);
        REQUIRE(packetData[i].y == data[i].y);
        REQUIRE(packetData[i].health == data[i].health);
    }
}
