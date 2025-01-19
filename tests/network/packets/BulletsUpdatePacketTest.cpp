#include "network/packets/BulletsUpdatePacket.hpp"
#include <SFML/System/Vector2.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Tests for BulletsUpdatePacket
TEST_CASE("BulletsUpdatePacket constructor initializes correctly", "[BulletsUpdatePacket]")
{
    std::vector<Network::BulletsUpdatePacket::BulletData> data;
    std::string id = "enemy_1234123415_1412";
    float vx = 234.13;
    float vy = 24.13;
    float x = 123;
    float y = 634;
    size_t size = 10;

    Network::BulletsUpdatePacket::BulletData bulletData({id, x, y, vx, vy});
    for (size_t i = 0; i < size; i++) {
        id.push_back('F');
        bulletData.x += 1;
        bulletData.y += 1;
        bulletData.vx += 1;
        bulletData.vy += 1;
        data.push_back(bulletData);
    }
    Network::BulletsUpdatePacket packet(data);

    REQUIRE(packet.getType() == Network::Packet::PacketType::BULLETS_UPDATE);
    auto packetData = packet.getData();
    REQUIRE(packetData.size() == data.size());
    for (auto i = 0; i < data.size(); i++) {
        REQUIRE(packetData[i].x == data[i].x);
        REQUIRE(packetData[i].y == data[i].y);
        REQUIRE(packetData[i].vx == data[i].vx);
        REQUIRE(packetData[i].vy == data[i].vy);
        REQUIRE(packetData[i].id == data[i].id);
    }
}

TEST_CASE("BulletsUpdatePacket serialization works correctly", "[BulletsUpdatePacket]")
{
    std::vector<Network::BulletsUpdatePacket::BulletData> data;
    std::string id = "enemy_1234123415_1412";
    float vx = 234.13;
    float vy = 24.13;
    float x = 123;
    float y = 634;
    size_t size = 10;

    Network::BulletsUpdatePacket::BulletData bulletData({id, x, y, vx, vy});
    for (size_t i = 0; i < size; i++) {
        id.push_back('F');
        bulletData.x += 1;
        bulletData.y += 1;
        bulletData.vx += 1;
        bulletData.vy += 1;
        data.push_back(bulletData);
    }
    Network::BulletsUpdatePacket packet(data);

    auto serializedData = packet.serialize();
    auto deserializedPacket = Network::BulletsUpdatePacket::deserialize(serializedData);

    REQUIRE(packet.getType() == Network::Packet::PacketType::BULLETS_UPDATE);
    auto packetData = deserializedPacket->getData();
    REQUIRE(packetData.size() == data.size());
    for (auto i = 0; i < data.size(); i++) {
        REQUIRE(packetData[i].x == data[i].x);
        REQUIRE(packetData[i].y == data[i].y);
        REQUIRE(packetData[i].vx == data[i].vx);
        REQUIRE(packetData[i].vy == data[i].vy);
        REQUIRE(packetData[i].id == data[i].id);
    }
}
