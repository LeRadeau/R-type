#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "Serializer.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/component/InputComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"
#include "ecs/system/HoverSystem.hpp"
#include "ecs/system/InputSystem.hpp"
#include "ecs/system/MenuSystem.hpp"
#include "ecs/system/MessageSystem.hpp"
#include "ecs/system/MovementSystem.hpp"
#include "ecs/system/RenderSystem.hpp"
#include "ecs/system/SelectionSystem.hpp"
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

static Entity &createPlayer(EntityManager &entityManager, const std::string &username)
{
    auto &playerEntity = entityManager.createEntity();
    playerEntity.addComponent<PositionComponent>(400, 300);
    playerEntity.addComponent<RenderComponent>(30, sf::Color::Green);
    playerEntity.addComponent<InputComponent>();
    playerEntity.addComponent<usernameComponent>(username);
    return playerEntity;
}

int main(void)
{
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ECS UDP Client");
    window.setFramerateLimit(60);

    EntityManager entityManager;

    RenderSystem renderSystem(window);
    HoverSystem hoverSystem;
    SelectionSystem selectionSystem;
    MovementSystem movementSystem;
    InputSystem inputSystem;
    MessageSystem messageSystem;

    MenuSystem menu(entityManager, font);

    menu.open();
    renderSystem.update(entityManager);
    window.display();
    sf::Clock deltaClock;

    std::string username = "user";
    std::string serverIp = "localhost";

    NetworkManager networkManager(serverIp, 54000);
    sendConnectMessage(networkManager, username);
    auto &i = createPlayer(entityManager, username);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (!window.hasFocus())
                break;
            if (event.type == sf::Event::MouseMoved) {
                hoverSystem.update(entityManager, event.mouseMove.x, event.mouseMove.y);
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                selectionSystem.update(entityManager, event.mouseButton);
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
                menu.toggle();
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
    sendGoodbyeMessage(networkManager, username);
    std::cout << "Goodbye" << std::endl;
    return 0;
}
