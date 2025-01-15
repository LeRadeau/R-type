#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/component/EventHandlerComponent.hpp"

class ButtonEntity {
  public:
    ButtonEntity(EntityManager &entityManager, sf::Vector2f size, sf::Vector2f position, const std::string &text,
        const sf::Font &font);
    void setCallback(sf::Event::EventType eventType, EventHandlerComponent::callbackFunction callback);
    Entity &getEntity();
    static Entity &createButtonEntity(EntityManager &entityManager, sf::Vector2f size, sf::Vector2f position,
        const std::string &text, const sf::Font &font);

  private:
    EntityManager &entityManager_;
    Entity &entity_;
};
