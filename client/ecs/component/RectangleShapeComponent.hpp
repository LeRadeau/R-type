#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include "ecs/component/AComponent.hpp"

struct RectangleShapeComponent : public AComponent {
    sf::RectangleShape shape;
    RectangleShapeComponent(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &color)
        : shape(size)
    {
        shape.setPosition(position);
        shape.setFillColor(color);
    }
};
