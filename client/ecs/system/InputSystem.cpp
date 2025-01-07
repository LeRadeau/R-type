#include "InputSystem.hpp"
#include "ecs/components.hpp"

void InputSystem::update(EntityManager &entityManager)
{
    for (auto &entity : entityManager.entities) {
        auto *input = entity->getComponent<InputComponent>();
        if (input) {
            input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
            input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        }
    }
}
