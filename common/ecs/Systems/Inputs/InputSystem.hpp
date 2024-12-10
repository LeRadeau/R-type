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
                for (int key = sf::Keyboard::A; key <= sf::Keyboard::KeyCount; ++key) {
                    auto keyEnum = static_cast<sf::Keyboard::Key>(key);
                    input->keyStates[keyEnum] = sf::Keyboard::isKeyPressed(keyEnum);
                }
            }
        }
    }
};
