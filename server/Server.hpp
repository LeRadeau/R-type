#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>

struct Client {
    sf::IpAddress ip;
    unsigned short port;
    std::string username;
    sf::Vector2f position;

    Client() : ip(), port(0), username(""), position(0, 0)
    {
    }
    Client(sf::IpAddress ipAddr, unsigned short p, const std::string &name)
        : ip(ipAddr), port(p), username(name), position(0, 0)
    {
    }
};

class Server {
  public:
    void bind(unsigned short port, const sf::IpAddress &addr = sf::IpAddress::Any);
    void run();

  private:
    void handleConnect(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleMove(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void handleGoodbye(const sf::IpAddress &sender, unsigned short senderPort, char const *&ptr);
    void readSocket();
    void updateClientPositions();
    sf::UdpSocket socket_;
    std::unordered_map<std::string, Client> clients_;
};
