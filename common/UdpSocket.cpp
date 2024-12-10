#include "UdpSocket.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>

void UdpSocket::sendTo(const Packet &data)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(data.getPort());
    inet_pton(AF_INET, data.getSenderIp().c_str(), &addr.sin_addr);
    sendto(m_sockfd, data.getBuffer(), data.getLength(), 0, (const struct sockaddr *) &addr, len);
}

std::vector<Packet> UdpSocket::receive()
{
    std::vector<Packet> packets;
    char buffer[65535]; // Maximum UDP packet size
    struct sockaddr_in senderAddr;
    socklen_t senderLen = sizeof(senderAddr);

    while (true) {
        ssize_t receivedBytes =
            recvfrom(m_sockfd, buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr *) &senderAddr, &senderLen);
        if (receivedBytes < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break;
            }
            throw std::runtime_error("Failed to receive data");
        }

        // Parse sender IP and port
        char senderIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &senderAddr.sin_addr, senderIp, INET_ADDRSTRLEN);
        uint16_t senderPort = ntohs(senderAddr.sin_port);

        // Validate packet size
        if (receivedBytes < sizeof(int) + sizeof(Packet::Type) + sizeof(size_t)) {
            std::cerr << "Malformed packet: too small to contain header, ignoring" << std::endl;
            continue;
        }

        // Extract type and size
        int player_id;
        Packet::Type type;
        size_t size;
        memcpy(&player_id, buffer, sizeof(player_id));
        memcpy(&type, buffer + sizeof(player_id), sizeof(type));
        memcpy(&size, buffer + sizeof(player_id) + sizeof(type), sizeof(size));

        // Validate payload size
        if (receivedBytes < sizeof(player_id) + sizeof(type) + sizeof(size) + size) {
            std::cerr << "Malformed packet: declared size does not match actual size, ignoring" << std::endl;
            continue;
        }

        // Extract payload
        void *data = nullptr;
        if (size > 0) {
            data = malloc(size);
            if (!data) {
                throw std::bad_alloc();
            }
            memcpy(data, buffer + sizeof(type) + sizeof(size), size);
        }

        // Construct the Packet object
        packets.emplace_back(senderIp, senderPort, player_id, type, size, data);

        // Free temporary memory (packet takes ownership of data)
        if (data) {
            free(data);
        }
    }
    return packets;
}
