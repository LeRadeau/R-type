#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>
#include <cstdint>

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

int main() {
    sf::UdpSocket socket;
    if (socket.bind(SERVER_PORT) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket on port " << SERVER_PORT << std::endl;
        return -1;
    }

    std::unordered_map<std::string, Client> clients;
    sf::IpAddress sender;
    unsigned short senderPort;

    std::cout << "Server is running on port " << SERVER_PORT << std::endl;

    while (true) {
        sf::Packet packet;
        if (socket.receive(packet, sender, senderPort) == sf::Socket::Done) {
            std::string messageType;
            packet >> messageType;

            if (messageType == "connect") {
                std::string newUsername;
                packet >> newUsername;

                if (clients.find(newUsername) != clients.end()) {
                    std::cout << "Client already exists: " << newUsername << std::endl;
                    sf::Packet errorPacket;
                    errorPacket << "error" << "Username already exists";
                    socket.send(errorPacket, sender, senderPort);
                    continue;
                } else {
                    clients[newUsername] = Client(sender, senderPort, newUsername);
                    std::cout << "New client connected: " << newUsername << std::endl;
                }


            } else if (messageType == "move") {
                std::string movingUsername;
                float x, y;
                packet >> movingUsername >> x >> y;

                if (clients.find(movingUsername) != clients.end()) {
                    clients[movingUsername].position = {x, y};
                }
            } else if (messageType == "goodbye") {
                std::string leavingUsername;
                packet >> leavingUsername;

                if (clients.find(leavingUsername) != clients.end()) {
                    clients.erase(leavingUsername);
                    std::cout << "Client disconnected: " << leavingUsername << std::endl;
                }
            }

            // Broadcast all client positions
            sf::Packet updatePacket;
            updatePacket << "update_clients";
            updatePacket << static_cast<uint32_t>(clients.size());

            for (const auto& [username, client] : clients) {
                updatePacket << client.username << client.position.x << client.position.y;
            }

            for (const auto& [username, client] : clients) {
                socket.send(updatePacket, client.ip, client.port);
            }
        }
    }

    return 0;
}
