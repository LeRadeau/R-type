#include "ParallaxSystem.hpp"
#include "ecs/component/ParallaxComponent.hpp"

void ParallaxSystem::update(EntityManager &entityManager, float deltaTime)
{
    for (auto &entity : entityManager.entities) {
        auto *parallax = entity->getComponent<ParallaxComponent>();

        if (parallax) {
            // Update position based on speed and deltaTime
            parallax->position.x += parallax->speed.x * deltaTime;
            parallax->position.y += parallax->speed.y * deltaTime;

            // Wrap positions to create seamless scrolling
            const auto textureSize = parallax->texture.getSize();
            if (parallax->position.x < -static_cast<float>(textureSize.x) * parallax->scale.x)
                parallax->position.x = 0;
            if (parallax->position.y < -static_cast<float>(textureSize.y) * parallax->scale.y)
                parallax->position.y = 0;

            // Update sprite position
            parallax->sprite.setPosition(parallax->position);
        }
    }
}

void ParallaxSystem::render(sf::RenderWindow &window, EntityManager &entityManager)
{
    for (auto &entity : entityManager.entities) {
        auto *parallax = entity->getComponent<ParallaxComponent>();

        if (parallax) {
            const auto textureSize = parallax->texture.getSize();

            // Render multiple tiles to fill the screen
            for (float x = parallax->position.x; x < static_cast<float>(window.getSize().x);
                 x += textureSize.x * parallax->scale.x) {
                for (float y = parallax->position.y; y < static_cast<float>(window.getSize().y);
                     y += textureSize.y * parallax->scale.y) {
                    parallax->sprite.setPosition(x, y);
                    window.draw(parallax->sprite);
                }
            }
        }
    }
}
