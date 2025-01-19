#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <memory>

struct ClientNetworking {
    std::shared_ptr<sf::TcpSocket> socket;
    sf::IpAddress ip;
    unsigned short port;
    ClientNetworking(std::shared_ptr<sf::TcpSocket> sock, unsigned short p)
        : socket(sock), ip(sock->getRemoteAddress()), port(p)
    {
    }
};
