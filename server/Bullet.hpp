#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>

struct Bullet {
    std::string id;
    sf::Vector2f position;
    sf::Vector2f velocity;
    std::string shooter;
};
