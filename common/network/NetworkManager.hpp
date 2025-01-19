#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <optional>
#include "TSQueue.hpp"
#include "packets/Packet.hpp"

namespace Network
{
    class NetworkManager {
      public:
        enum class Mode { SERVER, CLIENT };

        struct NetworkPacketInfo {
            enum class Protocol { TCP, UDP };
            std::shared_ptr<Packet> packet;
            Protocol protocol;
            std::optional<sf::IpAddress> address;                 // For UDP
            std::optional<unsigned short> port;                   // For UDP
            std::optional<std::shared_ptr<sf::TcpSocket>> socket; // For TCP
            NetworkPacketInfo(
                std::shared_ptr<Packet> packet, Protocol protocol, const sf::IpAddress &address, unsigned short port)
                : packet(std::move(packet)), protocol(protocol), address(address), port(port), socket(std::nullopt)
            {
            }
            NetworkPacketInfo(std::shared_ptr<Packet> packet, Protocol protocol, std::shared_ptr<sf::TcpSocket> socket)
                : packet(std::move(packet)), protocol(protocol), address(std::nullopt), port(std::nullopt),
                  socket(socket)
            {
            }
        };
        struct Notification {
            std::optional<std::exception> message;
            std::optional<sf::IpAddress> address;
            std::optional<unsigned short> port;
            std::optional<std::shared_ptr<sf::TcpSocket>> socket;

            Notification(const std::exception &e)
                : message(e), address(std::nullopt), port(std::nullopt), socket(std::nullopt)
            {
            }

            Notification(const std::exception &e, std::shared_ptr<sf::TcpSocket> socket)
                : message(e), address(std::nullopt), port(std::nullopt), socket(socket)
            {
            }

            Notification(const std::exception &e, sf::IpAddress address, unsigned short port)
                : message(e), address(address), port(port), socket(std::nullopt)
            {
            }
        };
        explicit NetworkManager(Mode mode);
        ~NetworkManager();
        void start();
        void stop();
        bool isRunning() const;

        std::optional<NetworkPacketInfo> getNextPacket();
        std::optional<Notification> getNextNotification();
        unsigned short getUdpPort() const;

        void sendPacket(const std::shared_ptr<Packet> &packet, const sf::IpAddress &address, unsigned short port);

        // Server specific
        void sendPacket(const std::shared_ptr<Packet> &packet, std::shared_ptr<sf::TcpSocket> socket);
        void listen(const sf::IpAddress &ip, unsigned short udpPort, unsigned short tcpPort);

        // Client specific
        void sendPacket(const std::shared_ptr<Packet> &packet); // For TCP only
        void connect(const sf::IpAddress &remote, unsigned short tcpPort, unsigned short udpPort);

      private:
        void tcpListenerThread();                                           // For server: Accept connections
        void tcpReceiveThread(std::shared_ptr<sf::TcpSocket> clientSocket); // For both: Handle incomming TCP data
        void udpReceiveThread();                                            // For both: Handle incomming UDP data
        void processOutgoingPacketsThread();                                // Shared: Send packets

        void processOutgoingTcpData(NetworkPacketInfo &packetInfo);
        void processOutgoingUdpData(NetworkPacketInfo &packetInfo);

        Mode m_mode;
        sf::UdpSocket m_udpSocket;
        std::shared_ptr<sf::TcpSocket> m_tcpSocket;
        sf::TcpListener m_tcpListener;
        TSQueue<NetworkPacketInfo> m_incomingPackets;
        TSQueue<NetworkPacketInfo> m_outGoingPackets;
        std::atomic<bool> m_running;
        std::thread m_networkThread;
        TSQueue<Notification> m_notifications;
    };
} // namespace Network
