#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "Menu.hpp"
#include "Serializer.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/component/InputComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"
#include "ecs/system/HoverSystem.hpp"
#include "ecs/system/InputSystem.hpp"
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

static void createPlayer(EntityManager &entityManager, const std::string &username)
{
    auto &playerEntity = entityManager.createEntity();
    playerEntity.addComponent<PositionComponent>(400, 300);
    playerEntity.addComponent<RenderComponent>(30, sf::Color::Green);
    playerEntity.addComponent<InputComponent>();
    playerEntity.addComponent<usernameComponent>(username);
}

static void gameLoop(
    sf::RenderWindow &window, EntityManager &entityManager, const std::string &serverIp, const std::string &username)
{
    NetworkManager networkManager(serverIp, 54000);
    sendConnectMessage(networkManager, username);

    createPlayer(entityManager, username);

    RenderSystem renderSystem(window);
    MovementSystem movementSystem;
    InputSystem inputSystem;
    MessageSystem messageSystem;

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

    EntityManager entityManager;
    HoverSystem hoverSystem;
    SelectionSystem selectionSystem;
    RenderSystem renderSystem(window);

    Menu menu(entityManager, font);

    menu.open();
    renderSystem.update(entityManager);
    window.display();

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
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::M) {
                menu.open();
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Q) {
                menu.close();
            }
        }
        window.clear();
        renderSystem.update(entityManager);
        window.display();
    }
    /* gameLoop(window, entityManager, serverIp, username); */

    return 0;
}
