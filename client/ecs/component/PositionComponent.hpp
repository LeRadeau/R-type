#pragma once
#include <SFML/System/Vector2.hpp>
#include "AComponent.hpp"

struct PositionComponent : public AComponent {
    sf::Vector2f position;
    PositionComponent(float x = 0, float y = 0) : position(x, y)
    {
    }
};
