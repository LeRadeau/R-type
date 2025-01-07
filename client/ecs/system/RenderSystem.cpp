#include "RenderSystem.hpp"
#include "ecs/components.hpp"

RenderSystem::RenderSystem(sf::RenderWindow &window, const std::string &fontName) : window(window)
{
    font.loadFromFile(fontName);
}

void RenderSystem::displayBarInfos(EntityManager &entityManager)
{
    sf::RectangleShape bar = sf::RectangleShape(sf::Vector2f(window.getSize().x, 70));
    bar.setFillColor(sf::Color::Blue);
    window.draw(bar);
    int offset = 0;
    int usernameOffset = 0;
    for (auto &entity : entityManager.entities) {
        auto *username = entity->getComponent<usernameComponent>();

        if (username) {
            sf::Text text;
            text.setFont(font);
            text.setString(username->username);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(offset, 10);
            window.draw(text);
            usernameOffset = (text.getLocalBounds().width + 10);
            text.setString("HP: 100");
            text.setPosition(offset, 40);
            window.draw(text);
            offset += (text.getLocalBounds().width + 10) < usernameOffset ? usernameOffset
                                                                          : (text.getLocalBounds().width + 10);
        }
    }
}

void RenderSystem::update(EntityManager &entityManager)
{
    for (auto &entity : entityManager.entities) {
        auto *position = entity->getComponent<PositionComponent>();
        auto *render = entity->getComponent<RenderComponent>();
        auto *username = entity->getComponent<usernameComponent>();
        auto *health = entity->getComponent<HealthComponent>();

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
        displayBarInfos(entityManager);
    }
}
