#pragma once

#include "../../entitiesManager.hpp"

class InputSystem {
public:

    void update(EntityManager& entityManager, float dt) {

        for (auto& entity : entityManager.entities) {
            auto* input = entity->getComponent<InputComponent>();
            if (input) {
                input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
                input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    inputPress = true;
                }
            }
        }
    }
    bool inputPress;
};