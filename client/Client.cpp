#include <SFML/Graphics.hpp>
#include <iostream>
#include "./ecs/ecs.hpp"
#include "./ecs/components.hpp"
#include "./ecs/systems.hpp"
#include "./ecs/network.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <server_ip> <username>" << std::endl;
        return 1;
    }

    std::string serverIp = av[1];
    std::string username = av[2];

    sf::Font font;
    if (!font.loadFromFile("./client/assets/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "ECS UDP Client");
    window.setFramerateLimit(60);

    NetworkManager networkManager(serverIp, 54000);
    networkManager.connect(username);

    EntityManager entityManager;
    
    // Player entity
    auto& playerEntity = entityManager.createEntity();
    playerEntity.addComponent<PositionComponent>(400, 300);
    playerEntity.addComponent<RenderComponent>(30, sf::Color::Green);
    playerEntity.addComponent<NetworkComponent>(username);
    playerEntity.addComponent<InputComponent>();

    // Systems
    RenderSystem renderSystem(window);
    MovementSystem movementSystem;
    InputSystem inputSystem;

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                networkManager.goodbye(username);
                break;
            }
        }

        movementSystem.update(entityManager, networkManager);
        inputSystem.update(entityManager);

        window.clear();
        renderSystem.update(entityManager);

        auto otherClients = networkManager.getOtherClients();
        for (const auto& [otherUsername, position] : otherClients) {
            if (otherUsername == username) continue;
            sf::CircleShape otherPlayer(20);
            sf::Text text(otherUsername, font, 12);
            otherPlayer.setFillColor(sf::Color::Blue);
            otherPlayer.setPosition(position);
            window.draw(otherPlayer);
            text.setPosition(position.x, position.y - 20);
            window.draw(text);
        }
        window.display();
    }
    return 0;
}