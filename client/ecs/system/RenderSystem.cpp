#include "RenderSystem.hpp"
#include "ecs/component/HealthComponent.hpp"
#include "ecs/component/PositionComponent.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/ScoreComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "ecs/component/TextComponent.hpp"
#include "ecs/component/UsernameComponent.hpp"

RenderSystem::RenderSystem(sf::RenderWindow &window, const std::string &fontName) : window(window)
{
    font.loadFromFile(fontName);
}

void RenderSystem::update(EntityManager &entityManager, const std::string &playerUsername)
{
    for (auto &entity : entityManager.entities) {
        auto *position = entity->getComponent<PositionComponent>();
        auto *render = entity->getComponent<RenderComponent>();
        auto *username = entity->getComponent<UsernameComponent>();
        auto *health = entity->getComponent<HealthComponent>();
        auto *rectangleShape = entity->getComponent<RectangleShapeComponent>();
        auto *textComponent = entity->getComponent<TextComponent>();
        auto *sprite = entity->getComponent<SpriteComponent>();
        auto *scoreComponent = entity->getComponent<ScoreComponent>();

        if (scoreComponent && username && username->username == playerUsername) {
            sf::Text text;
            text.setFont(font);
            text.setString("SCORE: " + std::to_string(scoreComponent->score));
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            text.setPosition(window.getSize().x / 2.0f, 10);
            window.draw(text);
        }
        if (health && health->health <= 0) // Gros Hack de merde car flemme de voir le code coté serveur.
            continue;
        if (rectangleShape)
            window.draw(rectangleShape->shape);
        if (textComponent)
            window.draw(textComponent->data);
        if (position && sprite && username && health) {
            sprite->sprite.setPosition(position->position);
            window.draw(sprite->sprite);

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
        } else if (position && sprite && health) {
            sprite->sprite.setPosition(position->position);
            window.draw(sprite->sprite);

            sf::Text text;
            text.setFont(font);
            text.setString("HP: " + std::to_string(health->health));
            text.setPosition(position->position.x, position->position.y + 30);
            window.draw(text);
        } else if (position && sprite && username && textComponent) {
            sprite->sprite.setPosition(position->position);
            textComponent->data.setPosition(position->position.x, position->position.y - 30);
            window.draw(sprite->sprite);
        } else if (position && render) {
            render->shape.setPosition(position->position);
            window.draw(render->shape);
        } else if (position && sprite) {
            sprite->sprite.setPosition(position->position);
            window.draw(sprite->sprite);
        }
    }
}
