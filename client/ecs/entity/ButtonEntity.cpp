#include "ButtonEntity.hpp"
#include "ecs/component/EventHandlerComponent.hpp"
#include "ecs/component/HoverComponent.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/component/TextComponent.hpp"

ButtonEntity::ButtonEntity(EntityManager &entityManager, sf::Vector2f size, sf::Vector2f position,
    const std::string &text, const sf::Font &font)
    : entityManager_(entityManager), entity_(entityManager.createEntity())
{
    entity_.addComponent<HoverComponent>(sf::Color(128, 128, 128), sf::Color(128, 128, 128, 128));
    entity_.addComponent<RectangleShapeComponent>(size, position, sf::Color(128, 128, 128));
    entity_.addComponent<TextComponent>(text, font, position, sf::Color::White);
}

void ButtonEntity::setCallback(sf::Event::EventType eventType, EventHandlerComponent::callbackFunction callback)
{
    auto *eventHandlerComponent = entity_.getComponent<EventHandlerComponent>();
    if (!eventHandlerComponent)
        eventHandlerComponent = &entity_.addComponent<EventHandlerComponent>(eventType, callback);
    eventHandlerComponent->setCallback(eventType, callback);
}

Entity &ButtonEntity::getEntity()
{
    return entity_;
}
