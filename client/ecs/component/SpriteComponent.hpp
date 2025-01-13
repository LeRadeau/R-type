#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include "AComponent.hpp"

struct SpriteComponent : public AComponent {
    sf::Sprite sprite;
    sf::Vector2f scale;

    SpriteComponent(sf::Texture &texture, const sf::Vector2f &scale) : sprite(texture), scale(scale)
    {
        sprite.setScale(scale);
    }
};
