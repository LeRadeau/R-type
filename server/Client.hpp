#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

struct Client {
    std::shared_ptr<sf::TcpSocket> socket;
    sf::IpAddress ip;
    unsigned short port;
    std::string username;
    sf::Vector2f position;
    int health;
    int score;
    bool isAlive;

    Client() : ip(), port(0), username(""), position(0, 0), health(0), score(0), isAlive(false)
    {
    }
    Client(std::shared_ptr<sf::TcpSocket> sock, unsigned short p, const std::string &name)
        : socket(sock), ip(sock->getRemoteAddress()), port(p), username(name), position(0, 0), health(100), score(0),
          isAlive(true)
    {
    }
};
