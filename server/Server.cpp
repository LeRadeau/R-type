#include "Server.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "./../common/Serializer.hpp"
#include "./../common/network_types.hpp"
#include <system_error>
#include <unordered_map>

#define SERVER_PORT 54000

void Server::bind(unsigned short port, const sf::IpAddress &addr)
{
    if (socket_.bind(SERVER_PORT, addr) != sf::Socket::Done) {
        throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
    }
    std::cout << "Server is running on port " << port << std::endl;
}

void Server::handleConnect(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string newUsername = Serializer::deserializeString(ptr);

    if (clients_.find(newUsername) != clients_.end()) {
        std::cout << "Client already exists: " << newUsername << std::endl;

        std::string errorBuffer;
        Serializer::serialize(errorBuffer, static_cast<uint8_t>(MessageType::ERROR));
        Serializer::serialize(errorBuffer, "Username already exists");
        socket_.send(errorBuffer.data(), errorBuffer.size(), sender, senderPort);
    }

    clients_[newUsername] = Client(sender, senderPort, newUsername);
    std::cout << "New client connected: " << newUsername << std::endl;
}

void Server::handleMove(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string movingUsername = Serializer::deserializeString(ptr);
    float x = Serializer::deserialize<float>(ptr);
    float y = Serializer::deserialize<float>(ptr);

    if (clients_.find(movingUsername) != clients_.end()) {
        clients_[movingUsername].position = {x, y};
    }
}

void Server::handleGoodbye(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr)
{
    std::string leavingUsername = Serializer::deserializeString(ptr);

    if (clients_.find(leavingUsername) != clients_.end()) {
        clients_.erase(leavingUsername);
        std::cout << "Client disconnected: " << leavingUsername << std::endl;
    }
}

void Server::readSocket()
{
    while (true) {
        char data[1024];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short senderPort;
        if (socket_.receive(data, sizeof(data), received, sender, senderPort) != sf::Socket::Done) {
            continue;
        }
        const char *ptr = data;
        auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

        switch (messageType) {
            case MessageType::CONNECT: handleConnect(sender, senderPort, ptr); break;
            case MessageType::MOVE: handleMove(sender, senderPort, ptr); break;
            case MessageType::GOODBYE: handleGoodbye(sender, senderPort, ptr); break;
            default: break;
        }
    }
    updateClientPositions();
}

void Server::updateClientPositions()
{
    // Broadcast updated client positions
    std::string updateBuffer;
    Serializer::serialize(updateBuffer, static_cast<uint8_t>(MessageType::UPDATE_CLIENTS));
    Serializer::serialize(updateBuffer, static_cast<uint32_t>(clients_.size()));

    for (const auto &[username, client] : clients_) {
        Serializer::serialize(updateBuffer, client.username);
        Serializer::serialize(updateBuffer, client.position.x);
        Serializer::serialize(updateBuffer, client.position.y);
    }

    for (const auto &[username, client] : clients_) {
        socket_.send(updateBuffer.data(), updateBuffer.size(), client.ip, client.port);
    }
}

void Server::run()
{
    std::thread networkThread(&Server::readSocket, this);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

int main()
{
    Server server;

    try {
        server.bind(SERVER_PORT);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    server.run();
}
