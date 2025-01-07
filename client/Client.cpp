#include <SFML/Graphics.hpp>
#include <iostream>
#include "Serializer.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/components.hpp"
#include "ecs/systems.hpp"
#include "network_types.hpp"

static void sendConnectMessage(NetworkManager &networkManager, const std::string &username)
{
    std::string buffer;
    Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::CONNECT));
    Serializer::serialize(buffer, username);
    networkManager.send(buffer);
}

static void sendGoodbyeMessage(NetworkManager &networkManager, const std::string &username)
{
    std::string buffer;
    Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::GOODBYE));
    Serializer::serialize(buffer, username);
    networkManager.send(buffer);
}

int main(int argc, char *const *argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <username>" << std::endl;
        return 1;
    }

    std::string serverIp = argv[1];
    std::string username = argv[2];

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ECS UDP Client");
    window.setFramerateLimit(60);

    NetworkManager networkManager(serverIp, 54000);

    EntityManager entityManager;

    sendConnectMessage(networkManager, username);

    auto &playerEntity = entityManager.createEntity();
    playerEntity.addComponent<PositionComponent>(400, 300);
    playerEntity.addComponent<RenderComponent>(30, sf::Color::Green);
    playerEntity.addComponent<NetworkComponent>(username);
    playerEntity.addComponent<InputComponent>();
    playerEntity.addComponent<usernameComponent>(username);

    RenderSystem renderSystem(window);
    MovementSystem movementSystem;
    InputSystem inputSystem;
    MessageSystem messageSystem;

    std::queue<std::string> receivedMessages;

    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                sendGoodbyeMessage(networkManager, username);
                break;
            }
        }
        float deltaTime = deltaClock.restart().asSeconds();

        movementSystem.update(entityManager, networkManager, deltaTime, window.hasFocus());
        inputSystem.update(entityManager);
        messageSystem.update(entityManager, networkManager, username);

        window.clear();
        renderSystem.update(entityManager);
        window.display();
    }
    return 0;
}
