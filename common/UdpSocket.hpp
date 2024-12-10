#pragma once
#include <cstring>
#include <stdexcept>
#include <vector>
#include "Packet.hpp"
#ifdef _WIN32
    #include <mstcpip.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

class UdpSocket {
  private:
    int m_sockfd;
    bool m_is_initialized;

#ifdef _WIN32
    WSADATA wsaData;
#endif

  public:
    UdpSocket() : m_is_initialized(false)
    {
#ifdef _WIN32
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("Failed to initialize Winsock");
        }
#endif
        m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_sockfd < 0) {
            cleanup();
            throw std::runtime_error("Failed to create socket");
        }
        m_is_initialized = true;
    }

    ~UdpSocket()
    {
        if (m_is_initialized) {
            cleanup();
        }
    }

    void bindSocket(const std::string &ip, uint16_t port)
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        if (bind(m_sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            throw std::runtime_error("Failed to bind socket");
        }
    }
    void sendTo(const Packet &data);
    std::vector<Packet> receive();

  private:
    void cleanup()
    {
#ifdef _WIN32
        closesocket(m_sockfd);
        WSACleanup();
#else
        close(m_sockfd);
#endif
        m_is_initialized = false;
    }
};
