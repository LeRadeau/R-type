#pragma once
#include "ecs.hpp"
#include "components.hpp"
#include "network.hpp"
#include <SFML/Graphics.hpp>

class RenderSystem {
public:
    RenderSystem(sf::RenderWindow& window) : window(window) {}

    void update(EntityManager& entityManager) {
        for (auto& entity : entityManager.entities) {
            auto* position = entity->getComponent<PositionComponent>();
            auto* render = entity->getComponent<RenderComponent>();
            
            if (position && render) {
                render->shape.setPosition(position->position);
                window.draw(render->shape);
            }
        }
    }

private:
    sf::RenderWindow& window;
};

class InputSystem {
public:
    void update(EntityManager& entityManager) {
        for (auto& entity : entityManager.entities) {
            auto* input = entity->getComponent<InputComponent>();
            if (input) {
                input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
                input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
            }
        }
    }
};

class MovementSystem {
public:
    void update(EntityManager& entityManager, NetworkManager& networkManager) {
        for (auto& entity : entityManager.entities) {
            auto* input = entity->getComponent<InputComponent>();
            auto* position = entity->getComponent<PositionComponent>();
            auto* network = entity->getComponent<NetworkComponent>();
            
            if (input && position && network) {
                if (input->moveLeft) 
                    position->position.x -= 1.0f;
                if (input->moveRight)
                    position->position.x += 1.0f;
                if (input->moveUp)
                    position->position.y -= 1.0f;
                if (input->moveDown)
                    position->position.y += 1.0f;
                
                networkManager.sendPlayerPosition(
                    network->username, 
                    position->position
                );
            }
        }
    }
};