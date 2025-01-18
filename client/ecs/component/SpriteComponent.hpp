#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "AComponent.hpp"

struct SpriteComponent : public AComponent {
    sf::Sprite sprite;
    sf::Vector2f scale;
    sf::Texture texture;

    SpriteComponent(const std::string &assetPath, const sf::Vector2f &scale) : scale(scale)
    {
        texture.loadFromFile(assetPath);
        sprite.setTexture(texture);
        sprite.setScale(scale);
        sf::Vector2f origin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
        sprite.setOrigin(origin);
    }
};
