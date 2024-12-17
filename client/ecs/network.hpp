#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "ecs.hpp"

class NetworkManager {
public:
    NetworkManager(const std::string& serverIp, uint16_t port) : 
        serverIp(serverIp), serverPort(port) {
        if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
            throw std::runtime_error("Failed to bind socket");
        }
        receiverThread = std::thread(&NetworkManager::receiveMessages, this);
    }

    ~NetworkManager() {
        if (receiverThread.joinable()) {
            receiverThread.join();
        }
    }

    void connect(const std::string& username) {
        sf::Packet connectPacket;
        connectPacket << "connect" << username;
        socket.send(connectPacket, serverIp, serverPort);
    }

    void goodbye(const std::string& username) {
        sf::Packet goodbyePacket;
        goodbyePacket << "goodbye" << username;
        socket.send(goodbyePacket, serverIp, serverPort);
    }

    void sendPlayerPosition(const std::string& username, const sf::Vector2f& position) {
        sf::Packet packet;
        packet << "move" << username << position.x << position.y;
        socket.send(packet, serverIp, serverPort);
    }

    std::unordered_map<std::string, sf::Vector2f> getOtherClients() {
        std::lock_guard<std::mutex> lock(clientsMutex);
        return otherClients;
    }

private:
    void receiveMessages() {
        sf::Packet packet;
        sf::IpAddress sender;
        unsigned short senderPort;

        while (true) {
            if (socket.receive(packet, sender, senderPort) == sf::Socket::Done) {
                std::string messageType;
                packet >> messageType;

                if (messageType == "update_clients") {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    uint32_t clientCount;
                    packet >> clientCount;
                    otherClients.clear();

                    for (uint32_t i = 0; i < clientCount; ++i) {
                        std::string otherUsername;
                        float x, y;
                        packet >> otherUsername >> x >> y;
                        otherClients[otherUsername] = {x, y};
                    }
                } else if (messageType == "error") {
                    std::string errorMessage;
                    packet >> errorMessage;
                    std::cerr << "Error: " << errorMessage << std::endl;
                }
            }
        }
    }

    sf::UdpSocket socket;
    sf::IpAddress serverIp;
    uint16_t serverPort;
    std::thread receiverThread;
    std::mutex clientsMutex;
    std::unordered_map<std::string, sf::Vector2f> otherClients;
};