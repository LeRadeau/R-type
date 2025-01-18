#include "Server.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>

void Server::handleIncomingPackets()
{
    running_ = true;
    while (running_) {
        auto packet = m_networkManager.getNextPacket();
        while (packet.has_value()) {
            auto unpackedPacket = packet.value();
            switch (unpackedPacket.packet->getType()) {
                case Network::Packet::PacketType::PLAYER_READY: handleReady(unpackedPacket); break;
                case Network::Packet::PacketType::PLAYER_CONNECT: handleConnect(unpackedPacket); break;
                case Network::Packet::PacketType::PLAYER_MOVE: handleMove(unpackedPacket); break;
                case Network::Packet::PacketType::PLAYER_DISCONNECT: handleDisconnect(unpackedPacket); break;
                case Network::Packet::PacketType::PLAYER_SHOOT: handleShoot(unpackedPacket); break;
                default: break;
            }
            packet = m_networkManager.getNextPacket();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

Server::Server(unsigned short tcpPort, unsigned short udpPort, const sf::IpAddress &ip)
    : m_networkManager(Network::NetworkManager::Mode::SERVER), m_tcpPort(tcpPort), m_udpPort(udpPort), m_ip(ip)
{
}

Server::~Server()
{
    running_ = false;
    m_networkManager.stop();
}

int main()
{
    Server server(SERVER_PORT, SERVER_PORT);

    try {
        server.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
