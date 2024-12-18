#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>
#include <thread>  // For threading
#include <mutex>   // For thread safety
#include "./../common/Serializer.hpp"
#include "./../common/network_types.hpp"

#define SERVER_PORT 54000

struct Client {
    sf::IpAddress ip;
    unsigned short port;
    std::string username;
    sf::Vector2f position;

    Client() : ip(), port(0), username(""), position(0, 0) {}
    Client(sf::IpAddress ipAddr, unsigned short p, const std::string& name)
        : ip(ipAddr), port(p), username(name), position(0, 0) {}
};

struct Bullet {
    sf::Vector2f position;
    std::string shooter;
    int id;
};

std::unordered_map<std::string, Client> clients;
std::mutex clientsMutex;
std::vector<Bullet> bullets;
std::mutex bulletsMutex;

void handleNetwork(sf::UdpSocket& socket) {
    sf::IpAddress sender;
    unsigned short senderPort;

    while (true) {
        char data[1024];
        std::size_t received;

        if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
            const char* ptr = data;
            auto messageType = static_cast<MessageType>(Serializer::deserialize<uint8_t>(ptr));

            std::lock_guard<std::mutex> lock(clientsMutex);
            std::lock_guard<std::mutex> lock2(bulletsMutex);

            if (messageType == MessageType::CONNECT) {
                std::string newUsername = Serializer::deserializeString(ptr);

                if (clients.find(newUsername) != clients.end()) {
                    std::cout << "Client already exists: " << newUsername << std::endl;

                    std::string errorBuffer;
                    Serializer::serialize(errorBuffer, static_cast<uint8_t>(MessageType::ERROR));
                    Serializer::serialize(errorBuffer, std::string("Username already exists"));
                    socket.send(errorBuffer.data(), errorBuffer.size(), sender, senderPort);
                    continue;
                }

                clients[newUsername] = Client(sender, senderPort, newUsername);
                std::cout << "New client connected: " << newUsername << std::endl;

            } else if (messageType == MessageType::MOVE) {
                std::string movingUsername = Serializer::deserializeString(ptr);
                float x = Serializer::deserialize<float>(ptr);
                float y = Serializer::deserialize<float>(ptr);

                if (clients.find(movingUsername) != clients.end()) {
                    clients[movingUsername].position = {x, y};
                }

            } else if (messageType == MessageType::GOODBYE) {
                std::string leavingUsername = Serializer::deserializeString(ptr);

                if (clients.find(leavingUsername) != clients.end()) {
                    clients.erase(leavingUsername);
                    std::cout << "Client disconnected: " << leavingUsername << std::endl;
                }

            } else if (messageType == MessageType::SHOOT) {
                std::string shootingUsername = Serializer::deserializeString(ptr);
                float x = Serializer::deserialize<float>(ptr);
                float y = Serializer::deserialize<float>(ptr);
                std::cout << "Client " << shootingUsername;
                std::cout << " shot at position (" << x << ", " << y << ")" << std::endl;
                bullets.push_back({{x, y}, shootingUsername, static_cast<int>(bullets.size())});
            }

            // Broadcast updated client positions
            std::string updateBuffer;
            Serializer::serialize(updateBuffer, static_cast<uint8_t>(MessageType::UPDATE_CLIENTS));
            Serializer::serialize(updateBuffer, static_cast<uint32_t>(clients.size()));

            for (const auto& [username, client] : clients) {
                Serializer::serialize(updateBuffer, client.username);
                Serializer::serialize(updateBuffer, client.position.x);
                Serializer::serialize(updateBuffer, client.position.y);
            }

            for (const auto& [username, client] : clients) {
                socket.send(updateBuffer.data(), updateBuffer.size(), client.ip, client.port);
            }
        }
    }
}

int main() {
    sf::UdpSocket socket;
    if (socket.bind(SERVER_PORT) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket on port " << SERVER_PORT << std::endl;
        return -1;
    }

    std::cout << "Server is running on port " << SERVER_PORT << std::endl;

    std::thread networkThread(handleNetwork, std::ref(socket));

    // Main game loop
    while (true) {
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    networkThread.join();
    return 0;
}
