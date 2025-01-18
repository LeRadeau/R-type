#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include "AComponent.hpp"

struct RenderComponent : public AComponent {
    sf::CircleShape shape;
    sf::Color color;

    RenderComponent(float radius, sf::Color color) : shape(radius), color(color)
    {
        shape.setFillColor(color);
        sf::Vector2f origin(shape.getLocalBounds().width / 2.0f, shape.getLocalBounds().height / 2.0f);
        shape.setOrigin(origin);
    }
};
