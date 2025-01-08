#include "HoverSystem.hpp"
#include "ecs/component/HoverComponent.hpp"
#include "ecs/component/RectangleShapeComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"

void HoverSystem::update(EntityManager &entityManager, float x, float y)
{
    for (auto &i : entityManager.entities) {
        auto *hoverComponent = i->getComponent<HoverComponent>();
        auto *spriteComponent = i->getComponent<SpriteComponent>();
        auto *rectangleShapeComponent = i->getComponent<RectangleShapeComponent>();
        if (hoverComponent && spriteComponent) {
            if (spriteComponent->sprite.getGlobalBounds().contains(x, y)) {
                spriteComponent->sprite.setColor(hoverComponent->hoverColor);
            } else {
                spriteComponent->sprite.setColor(hoverComponent->defaultColor);
            }
        }
        if (hoverComponent && rectangleShapeComponent) {
            if (rectangleShapeComponent->shape.getGlobalBounds().contains(x, y)) {
                rectangleShapeComponent->shape.setFillColor(hoverComponent->hoverColor);
            } else {
                rectangleShapeComponent->shape.setFillColor(hoverComponent->defaultColor);
            }
        }
    }
}
