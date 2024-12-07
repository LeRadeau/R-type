#pragma once

#include "../Component.hpp"

//Bibliothèque Graphique
#include <SFML/Graphics.hpp>

struct BackgroundComponent : public Component {
    sf::Sprite sprite;
    float parallaxSpeed;
    float scale;

    BackgroundComponent(const sf::Texture& texture, float parallaxSpeed = 0, float scale = 1.0f) : parallaxSpeed(parallaxSpeed), scale(scale) {
        sprite.setTexture(texture);
        sprite.setScale(scale, scale);
    }
};