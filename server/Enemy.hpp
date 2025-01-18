#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>

struct Enemy {
    std::string id;
    sf::Vector2f position;
    sf::Vector2f velocity;
    int health;
    float shootingCooldown;
    float respawnCooldown;
    float startingY;
    bool isAlive;
    float amplitude;
    float frequency;
    float cosinus;
};
