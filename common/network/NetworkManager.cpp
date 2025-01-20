#include "NetworkManager.hpp"
#include <SFML/Network/SocketSelector.hpp>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include "network/exceptions/NetworkExceptions.hpp"

namespace Network
{
    NetworkManager::NetworkManager(Mode mode) : m_mode(mode)
    {
    }

    NetworkManager::~NetworkManager()
    {
        if (isRunning())
            stop();
    }

    void NetworkManager::start()
    {
        m_running = true;

        if (m_mode == Mode::SERVER) {
            m_networkThread = std::thread(&NetworkManager::tcpListenerThread, this);
        } else {
            m_networkThread = std::thread(&NetworkManager::tcpReceiveThread, this, m_tcpSocket);
        }
        std::thread(&NetworkManager::udpReceiveThread, this).detach();
        std::thread(&NetworkManager::processOutgoingPacketsThread, this).detach();
    }

    void NetworkManager::stop()
    {
        m_running = false;

        m_udpSocket.unbind();
        if (m_mode == Mode::SERVER)
            m_tcpListener.close();
        else
            m_tcpSocket->disconnect();

        if (m_networkThread.joinable())
            m_networkThread.join();
    }

    void NetworkManager::sendPacket(
        const std::shared_ptr<Packet> &packet, const sf::IpAddress &remote, unsigned short udpPort)
    {
        std::cout << "Sending UDP packet " << packet->getType() << " to " << remote << ":" << udpPort << std::endl;
        m_outGoingPackets.push(NetworkPacketInfo(packet, NetworkPacketInfo::Protocol::UDP, remote, udpPort));
    }

    void NetworkManager::sendPacket(const std::shared_ptr<Packet> &packet, const std::shared_ptr<sf::TcpSocket> &socket)
    {
        if (!socket || socket->getRemotePort() == 0)
            throw NetworkManagerException("TCP socket is gone: Mode::SERVER");
        std::cout << "Sending TCP packet " << packet->getType() << " to " << socket->getRemoteAddress() << ":"
                  << socket->getRemotePort() << std::endl;
        m_outGoingPackets.push(NetworkPacketInfo(packet, NetworkPacketInfo::Protocol::TCP, socket));
    }

    void NetworkManager::sendPacket(const std::shared_ptr<Packet> &packet)
    {
        if (!m_tcpSocket || m_tcpSocket->getRemotePort() == 0)
            throw NetworkManagerException("TCP socket is gone: Mode::CLIENT");
        std::cout << "Sending TCP packet " << packet->getType() << " to " << m_tcpSocket->getRemoteAddress() << ":"
                  << m_tcpSocket->getRemotePort() << std::endl;
        if (m_mode != Mode::CLIENT)
            throw std::logic_error("sendPacket can not be called without destination in server mode");
        m_outGoingPackets.push(NetworkPacketInfo(packet, NetworkPacketInfo::Protocol::TCP, m_tcpSocket));
    }

    bool NetworkManager::isRunning() const
    {
        return m_running;
    }

    std::optional<NetworkManager::NetworkPacketInfo> NetworkManager::getNextPacket()
    {
        if (m_incomingPackets.empty())
            return std::nullopt;
        return m_incomingPackets.pop();
    }

    std::optional<NetworkManager::Notification> NetworkManager::getNextNotification()
    {
        if (m_notifications.empty())
            return std::nullopt;
        return m_notifications.pop();
    }

    unsigned short NetworkManager::getUdpPort() const
    {
        return m_udpSocket.getLocalPort();
    }

    void NetworkManager::listen(const sf::IpAddress &ip, unsigned short udpPort, unsigned short tcpPort)
    {
        if (m_mode != Mode::SERVER)
            throw std::logic_error("Listen can only be called in server mode");

        if (m_udpSocket.bind(udpPort, ip) != sf::Socket::Done)
            throw SocketBindingException("Failed to bind UDP socket to port " + std::to_string(udpPort));

        if (m_tcpListener.listen(tcpPort, ip) != sf::Socket::Done)
            throw SocketListeningException("Failed to listen on TCP port " + std::to_string(tcpPort));
    }

    void NetworkManager::connect(const sf::IpAddress &remote, unsigned short tcpPort, unsigned short udpPort)
    {
        if (m_mode != Mode::CLIENT)
            throw std::logic_error("Connect can only be called in client mode");

        if (m_udpSocket.bind(udpPort) != sf::Socket::Done)
            throw SocketBindingException("Failed to bind UDP socket to port " + std::to_string(udpPort));

        m_tcpSocket = std::make_shared<sf::TcpSocket>();
        if (m_tcpSocket->connect(remote, tcpPort) != sf::Socket::Done)
            throw ConnectionException(
                "Failed to connect to remote at " + remote.toString() + ":" + std::to_string(tcpPort));
    }

    void NetworkManager::tcpListenerThread()
    {
        sf::SocketSelector selector;

        selector.add(m_tcpListener);
        while (m_running) {
            auto clientSocket = std::make_shared<sf::TcpSocket>();
            if (selector.wait(sf::seconds(1)) && selector.isReady(m_tcpListener)
                && m_tcpListener.accept(*clientSocket) == sf::Socket::Done) {
                std::cout << "Got new connection: " << clientSocket->getRemoteAddress() << ":"
                          << clientSocket->getRemotePort() << std::endl;
                std::thread(&NetworkManager::tcpReceiveThread, this, clientSocket).detach();
            }
        }
    }

    void NetworkManager::tcpReceiveThread(std::shared_ptr<sf::TcpSocket> socket)
    {
        sf::SocketSelector selector;

        selector.add(*socket);
        while (m_running && socket) {
            std::size_t received;
            if (selector.wait(sf::milliseconds(100))) {
                if (selector.isReady(*socket)) {
                    size_t size;
                    try {
                        sf::Socket::Status status = socket->receive(&size, sizeof(size), received);
                        switch (status) {
                            case sf::Socket::Partial: throw TransmissionException("Data was lost in TCP"); break;
                            case sf::Socket::Disconnected: throw ConnectionException("Socket disconnected"); break;
                            case sf::Socket::Error: throw TransmissionException("TCP Socket Error"); break;
                            default: break;
                        }
                        std::vector<uint8_t> data(size);

                        status = socket->receive(data.data(), data.size(), received);
                        data.resize(received);
                        switch (status) {
                            case sf::Socket::Partial: throw TransmissionException("Data was lost in TCP"); break;
                            case sf::Socket::Disconnected: throw ConnectionException("Socket disconnected"); break;
                            case sf::Socket::Error: throw TransmissionException("TCP Socket Error"); break;
                            default: break;
                        }
                        auto packet = Packet::deserialize(data);
                        m_incomingPackets.push(
                            NetworkPacketInfo(std::move(packet), NetworkPacketInfo::Protocol::TCP, socket));
                    } catch (std::exception &e) {
                        m_notifications.push(Notification(e, socket));
                    }
                }
            }
        }
    }

    void NetworkManager::udpReceiveThread()
    {
        sf::SocketSelector selector;

        selector.add(m_udpSocket);
        while (m_running) {
            if (selector.wait(sf::milliseconds(100))) {
                if (selector.isReady(m_udpSocket)) {
                    sf::IpAddress addr;
                    unsigned short port;
                    size_t received;
                    std::vector<uint8_t> data(4096);
                    try {
                        if (m_udpSocket.receive(data.data(), data.size(), received, addr, port) != sf::Socket::Done)
                            throw TransmissionException("Data was lost in udp");
                        data.resize(received);
                        auto packet = Packet::deserialize(data);
                        m_incomingPackets.push(NetworkPacketInfo(
                            std::move(packet), NetworkManager::NetworkPacketInfo::Protocol::UDP, addr, port));
                    } catch (std::exception &e) {
                        m_notifications.push(Notification(e, addr, port));
                    }
                }
            }
        }
    }

    void NetworkManager::processOutgoingPacketsThread()
    {
        while (m_running) {
            while (!m_outGoingPackets.empty()) {
                NetworkPacketInfo packetInfo = m_outGoingPackets.pop();
                try {
                    switch (packetInfo.protocol) {
                        case NetworkPacketInfo::Protocol::TCP: processOutgoingTcpData(packetInfo); break;
                        case NetworkPacketInfo::Protocol::UDP: processOutgoingUdpData(packetInfo); break;
                        default: throw NetworkManagerException("Malformed NetworkPacketInfo");
                    }
                } catch (const std::exception &e) {
                    m_notifications.push(Notification(e, packetInfo.socket.value()));
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void NetworkManager::processOutgoingTcpData(NetworkPacketInfo &packetInfo)
    {
        if (!packetInfo.socket)
            throw NetworkManagerException("Malformed TCP NetworkPacketInfo");
        std::vector<uint8_t> data = packetInfo.packet->serialize();
        std::size_t dataSize = data.size();
        if (packetInfo.socket->get()->send(&dataSize, sizeof(dataSize)) != sf::Socket::Done)
            throw TransmissionException("Failed to send packet size to " + m_tcpSocket->getRemoteAddress().toString()
                + ":" + std::to_string(m_tcpSocket->getRemotePort()));

        if (packetInfo.socket->get()->send(data.data(), data.size()) != sf::Socket::Done)
            throw TransmissionException("Failed to send packet to " + m_tcpSocket->getRemoteAddress().toString() + ":"
                + std::to_string(m_tcpSocket->getRemotePort()));
    }

    void NetworkManager::processOutgoingUdpData(NetworkPacketInfo &packetInfo)
    {
        if (!packetInfo.address || !packetInfo.port)
            throw NetworkManagerException("Malformed UDP NetworkPacketInfo");
        std::vector<uint8_t> data = packetInfo.packet->serialize();
        if (m_udpSocket.send(data.data(), data.size(), packetInfo.address.value(), packetInfo.port.value())
            != sf::Socket::Done)
            throw TransmissionException("Failed to send packet to " + packetInfo.address.value().toString() + ":"
                + std::to_string(packetInfo.port.value()));
    }

} // namespace Network
