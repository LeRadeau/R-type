#pragma once

#include <SFML/Window/Event.hpp>
#include <functional>
#include "AComponent.hpp"
#include <unordered_map>

struct EventHandlerComponent : public AComponent {
    using callbackFunction = std::function<void(const sf::Event &)>;
    std::unordered_map<sf::Event::EventType, callbackFunction> callbacks;

    EventHandlerComponent(sf::Event::EventType type, callbackFunction f)
    {
        callbacks[type] = f;
    }

    void setCallback(sf::Event::EventType type, callbackFunction f)
    {
        callbacks[type] = f;
    }
};
