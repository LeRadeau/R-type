#include "NetworkManager.hpp"
#include <cstdlib>
#include "Serializer.hpp"
#include <iostream>
// Public

NetworkManager::NetworkManager(const std::string &serverIp, uint16_t port)
    : serverIp(serverIp), serverPort(port), isRunning(true)
{
    socket.setBlocking(false);
    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        throw std::runtime_error("Failed to bind socket");
    }
    receiverThread = std::thread(&NetworkManager::receiveMessages, this);
}
NetworkManager::~NetworkManager()
{
    isRunning = false;
    if (receiverThread.joinable()) {
        receiverThread.join();
    }
}

void NetworkManager::send(const std::string &buffer)
{
    socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
}

void NetworkManager::send(MessageType type, const std::string &data)
{
    std::string buffer;
    Serializer::serialize(buffer, static_cast<uint8_t>(type));
    Serializer::serialize(buffer, data);
    send(buffer);
}

TSQueue<std::string> &NetworkManager::getReceivedMessages()
{
    return receivedMessages;
}

void NetworkManager::setRemoteIp(const std::string &ip)
{
    serverIp = ip;
}

void NetworkManager::setRemotePort(uint16_t port)
{
    serverPort = port;
}

// Private

void NetworkManager::receiveMessages()
{
    char data[1024];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short senderPort;

    while (isRunning) {
        if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
            receivedMessages.push(std::string(data, received));
        }
    }
}
