#pragma once
#include <SFML/Network.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

class NetworkManager {
  public:
    NetworkManager(const std::string &serverIp, uint16_t port);

    ~NetworkManager();

    void send(const std::string &buffer);

    std::queue<std::string> &getReceivedMessages();

  private:
    void receiveMessages();

    sf::UdpSocket socket;
    sf::IpAddress serverIp;
    uint16_t serverPort;
    std::thread receiverThread;

    std::mutex queueMutex;
    std::queue<std::string> receivedMessages;
};
