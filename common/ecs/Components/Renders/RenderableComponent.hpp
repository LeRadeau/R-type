#pragma once

#include "../Component.hpp"

//Bibliothèque Graphique
#include <SFML/Graphics.hpp>

struct RenderableComponent : public Component {
    sf::Sprite sprite;
    float scale;

    RenderableComponent(const sf::Texture& texture, float scale = 1.0f) : scale(scale) {
        sprite.setTexture(texture);
        sprite.setScale(scale, scale);
    }
};
