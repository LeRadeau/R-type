#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include "AComponent.hpp"

struct TextComponent : public AComponent {
    sf::Text data;

    TextComponent(const std::string &text, const sf::Font &font, const sf::Vector2f &position, const sf::Color &color)
    {
        data.setString(text);
        data.setFont(font);
        data.setPosition(position);
        data.setFillColor(color);
    }
};
