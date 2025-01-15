#pragma once
#include "AComponent.hpp"
#include "SFML/Graphics.hpp"

struct ParallaxComponent : public AComponent {
    sf::Vector2f speed;
    sf::Vector2f position;
    sf::Vector2u scale;
    sf::Texture texture;
    sf::Sprite sprite;

    ParallaxComponent(const std::string &texturePath, const sf::Vector2f &speed, const sf::Vector2f &position, const sf::Vector2u &scale)
        : speed(speed), position(position), scale(scale) 
    {
        texture.loadFromFile(texturePath);
        sprite.setTexture(texture);
        sprite.setScale(scale.x, scale.y);
        sprite.setPosition(position);
    }
};
