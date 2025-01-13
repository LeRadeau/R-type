#include "RenderSystem.hpp"
#include "ecs/component/HealthComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/TextComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"

RenderSystem::RenderSystem(sf::RenderWindow &window, const std::string &fontName) : window(window)
{
    font.loadFromFile(fontName);
}

void RenderSystem::update(EntityManager &entityManager)
{
    for (auto &entity : entityManager.entities) {
        auto *position = entity->getComponent<PositionComponent>();
        auto *render = entity->getComponent<RenderComponent>();
        auto *username = entity->getComponent<usernameComponent>();
        auto *health = entity->getComponent<HealthComponent>();
        auto *rectangleShape = entity->getComponent<RectangleShapeComponent>();
        auto *textComponent = entity->getComponent<TextComponent>();

        if (rectangleShape)
            window.draw(rectangleShape->shape);
        if (textComponent)
            window.draw(textComponent->data);
        if (position && render && username && health) {
            render->shape.setPosition(position->position);
            window.draw(render->shape);

            sf::Text text;
            text.setFont(font);
            text.setString(username->username);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(position->position.x, position->position.y - 30);
            window.draw(text);

            text.setString("HP: " + std::to_string(health->health));
            text.setPosition(position->position.x, position->position.y + 30);
            window.draw(text);
        } else if (position && render && health) {
            render->shape.setPosition(position->position);
            window.draw(render->shape);

            sf::Text text;
            text.setFont(font);
            text.setString("HP: " + std::to_string(health->health));
            text.setPosition(position->position.x, position->position.y + 30);
            window.draw(text);
        } else if (position && render && username) {
            render->shape.setPosition(position->position);
            window.draw(render->shape);

            sf::Text text;
            text.setFont(font);
            text.setString(username->username);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(position->position.x, position->position.y - 30);
            window.draw(text);
        } else if (position && render) {
            render->shape.setPosition(position->position);
            window.draw(render->shape);
        }
    }
}
