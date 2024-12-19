#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>

struct RenderComponent : public Component{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::String pathTexture;

    RenderComponent(sf::String pathTexture) : texture(texture), sprite(sprite), pathTexture(pathTexture) {};
};