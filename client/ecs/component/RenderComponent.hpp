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
    }
};
