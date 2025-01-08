#include "HoverSystem.hpp"
#include "ecs/component/HoverComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"

void HoverSystem::update(EntityManager &entityManager, float x, float y)
{
    for (auto &i : entityManager.entities) {
        auto *hoverComponent = i->getComponent<HoverComponent>();
        auto *spriteComponent = i->getComponent<SpriteComponent>();
        if (hoverComponent && spriteComponent)
            hoverComponent->isHover = spriteComponent->sprite.getGlobalBounds().contains(x, y);
    }
}
