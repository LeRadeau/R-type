#pragma once
#include <SFML/Network.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include "network_types.hpp"

class NetworkManager {
  public:
    NetworkManager(const std::string &serverIp, uint16_t port);

    ~NetworkManager();

    void send(const std::string &buffer);
    void send(MessageType type, const std::string &data);
    void setRemoteIp(const std::string &ip);
    void setRemotePort(uint16_t port);

    std::queue<std::string> &getReceivedMessages();

  private:
    void receiveMessages();

    sf::UdpSocket socket;
    sf::IpAddress serverIp;
    uint16_t serverPort;
    std::thread receiverThread;
    std::atomic<bool> isRunning;

    std::mutex queueMutex;
    std::queue<std::string> receivedMessages;
};
