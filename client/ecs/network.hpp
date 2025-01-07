#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <string>
#include <iostream>

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

    void send(const std::string& buffer) {
        socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
    }

    std::queue<std::string> getReceivedMessages() {
        std::lock_guard<std::mutex> lock(queueMutex);
        return receivedMessages;
    }

private:
    void receiveMessages() {
        char data[1024];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short senderPort;

        while (true) {
            if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
                std::lock_guard<std::mutex> lock(queueMutex);
                receivedMessages.push(std::string(data, received));
                if (receivedMessages.size() > 10) {
                    receivedMessages = std::queue<std::string>();
                }
            }
        }
    }

    sf::UdpSocket socket;
    sf::IpAddress serverIp;
    uint16_t serverPort;
    std::thread receiverThread;

    std::mutex queueMutex;
    std::queue<std::string> receivedMessages;
};
