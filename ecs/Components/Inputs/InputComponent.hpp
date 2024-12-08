#pragma once

#include "../Component.hpp"

#include <map>
#include <SFML/Window/Keyboard.hpp>

struct InputComponent : public Component {
    std::map<sf::Keyboard::Key, bool> keyStates;

    InputComponent() {
        // Initialiser toutes les touches avec `false`
        for (int key = sf::Keyboard::A; key <= sf::Keyboard::KeyCount; ++key) {
            keyStates[static_cast<sf::Keyboard::Key>(key)] = false;
        }
    }
};
