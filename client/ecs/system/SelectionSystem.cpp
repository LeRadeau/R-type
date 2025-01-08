#include "SelectionSystem.hpp"
#include "ecs/component/SelectionComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"

void SelectionSystem::update(EntityManager &entityManager, const sf::Event::MouseButtonEvent &mouseEvent)
{
    if (mouseEvent.button != sf::Mouse::Left)
        return;
    for (auto &i : entityManager.entities) {
        auto *spriteComponent = i->getComponent<SpriteComponent>();
        auto *selectionComponent = i->getComponent<SelectionComponent>();
        if (spriteComponent && selectionComponent)
            selectionComponent->isSelected =
                spriteComponent->sprite.getGlobalBounds().contains(mouseEvent.x, mouseEvent.y);
    }
}
