#pragma once
#include <SFML/System/Vector2.hpp>
#include "AComponent.hpp"

struct VelocityComponent : public AComponent {
    sf::Vector2f velocity;
    VelocityComponent(float x = 0, float y = 0) : velocity(x, y)
    {
    }
};
