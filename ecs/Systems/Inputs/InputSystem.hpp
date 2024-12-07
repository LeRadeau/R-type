#pragma once

//Bibliothèque Graphique
#include <SFML/Graphics.hpp>

// Entities Components
#include "../../Entities/EntityManager.hpp"

// Inputs Components
#include "../../Components/Inputs/InputComponent.hpp"

class InputSystem {
private:
    sf::RenderWindow& window;

public:
    explicit InputSystem(sf::RenderWindow& window) : window(window) {}

    void processInput(EntityManager& em) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        auto entities = em.getEntitiesWithComponent<InputComponent>("input");
        for (Entity e : entities) {
            auto input = em.getComponent<InputComponent>(e, "input");
            if (input) {
                input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
                input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
                input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
                input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
                input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
            }
        }
    }
};