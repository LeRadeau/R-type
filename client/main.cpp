#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "ecs/EntityManager.hpp"
#include "ecs/NetworkManager.hpp"
#include "ecs/entity/MenuEntity.hpp"
#include "ecs/entity/PlayerEntity.hpp"
#include "ecs/system/EventHandlingSystem.hpp"
#include "ecs/system/HoverSystem.hpp"
#include "ecs/system/InputSystem.hpp"
#include "ecs/system/MessageSystem.hpp"
#include "ecs/system/MovementSystem.hpp"
#include "ecs/system/RenderSystem.hpp"
#include "ecs/system/SelectionSystem.hpp"
#include "ecs/system/ParallaxSystem.hpp"
#include "network_types.hpp"

static void loadParallax(EntityManager &entityManager)
{
    auto &background = entityManager.createEntity();
    background.addComponent<ParallaxComponent>("assets/back.png", sf::Vector2f(-500.0f, 0.0f), sf::Vector2f(0, 0), sf::Vector2u(7, 7));

    auto &midground = entityManager.createEntity();
    midground.addComponent<ParallaxComponent>("assets/starBack.png", sf::Vector2f(-600.0f, 0.0f), sf::Vector2f(0, 0), sf::Vector2u(7, 7));
}

int main(int argc, char *const *argv)
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
    MessageSystem messageSystem(font);
    EventHandlingSystem eventHandlingSystem;
    ParallaxSystem parallaxSystem;

    renderSystem.update(entityManager);
    window.display();
    sf::Clock deltaClock;

    std::string serverIp = argc == 2 ? argv[1] : "localhost";

    std::unique_ptr<PlayerEntity> player;
    loadParallax(entityManager);

    NetworkManager networkManager(serverIp, 54000);
    MenuEntity menu(entityManager, window, font, player, networkManager);

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
            eventHandlingSystem.update(entityManager, event);
        }

        float deltaTime = deltaClock.restart().asSeconds();

        movementSystem.update(entityManager, networkManager, deltaTime, window.hasFocus());
        inputSystem.update(entityManager);
        messageSystem.update(entityManager, networkManager, menu.getUsername());
        parallaxSystem.update(entityManager, deltaTime);

        window.clear();
        parallaxSystem.render(window, entityManager);
        renderSystem.update(entityManager);
        window.display();
    }
    networkManager.send(MessageType::GOODBYE, menu.getUsername());
    return 0;
}
