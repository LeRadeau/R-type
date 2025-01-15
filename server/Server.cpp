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
#include <sstream>
#include <chrono>

void Server::bind(unsigned short port, const sf::IpAddress &addr)
{
    if (socket_.bind(SERVER_PORT, addr) != sf::Socket::Done) {
        throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
    }
    std::cout << "Server is running on port " << port << std::endl;
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
            case MessageType::READY: handleReady(sender, senderPort, ptr); break;
            case MessageType::CONNECT: handleConnect(sender, senderPort, ptr); break;
            case MessageType::MOVE: handleMove(sender, senderPort, ptr); break;
            case MessageType::GOODBYE: handleGoodbye(sender, senderPort, ptr); break;
            case MessageType::SHOOT: handleShoot(sender, senderPort, ptr); break;
            default: break;
        }
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
