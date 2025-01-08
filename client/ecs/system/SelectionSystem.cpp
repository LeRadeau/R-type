#include "SelectionSystem.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/component/SelectionComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"

void SelectionSystem::update(EntityManager &entityManager, const sf::Event::MouseButtonEvent &mouseEvent)
{
    if (mouseEvent.button != sf::Mouse::Left)
        return;
    for (auto &i : entityManager.entities) {
        auto *spriteComponent = i->getComponent<SpriteComponent>();
        auto *selectionComponent = i->getComponent<SelectionComponent>();
        auto *rectangleShapeComponent = i->getComponent<RectangleShapeComponent>();

        if (spriteComponent && selectionComponent) {
            if (spriteComponent->sprite.getGlobalBounds().contains(mouseEvent.x, mouseEvent.y)) {
                spriteComponent->sprite.setColor(selectionComponent->selectedColor);
            } else {
                spriteComponent->sprite.setColor(selectionComponent->defaultColor);
            }
        }
        if (rectangleShapeComponent && selectionComponent) {
            if (rectangleShapeComponent->shape.getGlobalBounds().contains(mouseEvent.x, mouseEvent.y)) {
                rectangleShapeComponent->shape.setFillColor(selectionComponent->selectedColor);
            } else {
                rectangleShapeComponent->shape.setFillColor(selectionComponent->defaultColor);
            }
        }
    }
}
