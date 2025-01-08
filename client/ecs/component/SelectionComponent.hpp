#pragma once
#include <SFML/Graphics/Color.hpp>
#include "AComponent.hpp"

struct SelectionComponent : public AComponent {
    sf::Color selectedColor;
    sf::Color defaultColor;

    SelectionComponent(const sf::Color &defaultColor, const sf::Color &selectedColor)
        : selectedColor(selectedColor), defaultColor(defaultColor)
    {
    }
};
