#pragma once

#include <SFML/Graphics/Color.hpp>
#include "ecs/component/AComponent.hpp"

struct HoverComponent : public AComponent {
    sf::Color hoverColor;
    sf::Color defaultColor;

    HoverComponent(const sf::Color &defaultColor, const sf::Color &hoverColor)
        : hoverColor(hoverColor), defaultColor(defaultColor)
    {
    }
};
