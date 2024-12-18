#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "./../../common/Serializer.hpp"
#include "./../../common/network_types.hpp"

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
        std::string buffer;
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::CONNECT));
        Serializer::serialize(buffer, username);
        socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
    }

    void goodbye(const std::string& username) {
        std::string buffer;
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::GOODBYE));
        Serializer::serialize(buffer, username);
        socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
    }

    void sendPlayerPosition(const std::string& username, const sf::Vector2f& position) {
        std::string buffer;
        Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::MOVE));
        Serializer::serialize(buffer, username);
        Serializer::serialize(buffer, position.x);
        Serializer::serialize(buffer, position.y);
        socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
    }

    std::unordered_map<std::string, sf::Vector2f> getOtherClients() {
        std::lock_guard<std::mutex> lock(clientsMutex);
        return otherClients;
    }

private:
    void receiveMessages() {
        char data[1024];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short senderPort;

        while (true) {
            if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
                const char* ptr = data;
                auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

                if (messageType == MessageType::UPDATE_CLIENTS) {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    uint32_t clientCount = Serializer::deserialize<uint32_t>(ptr);
                    otherClients.clear();

                    for (uint32_t i = 0; i < clientCount; ++i) {
                        std::string username = Serializer::deserializeString(ptr);
                        float x = Serializer::deserialize<float>(ptr);
                        float y = Serializer::deserialize<float>(ptr);
                        otherClients[username] = {x, y};
                    }
                } else if (messageType == MessageType::ERROR) {
                    std::string errorMessage = Serializer::deserializeString(ptr);
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
