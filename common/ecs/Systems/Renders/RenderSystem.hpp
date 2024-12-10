#pragma once

//Bibliothèque Graphique
#include <SFML/Graphics.hpp>

// Entities Components
#include "../../Entities/EntityManager.hpp"

// Transforms Components
#include "../../Components/Transforms/PositionComponent.hpp"

// Renders Components
#include "../../Components/Renders/BackgroundComponent.hpp"
#include "../../Components/Renders/RenderableComponent.hpp"

class RenderSystem {
private:
    sf::RenderWindow& window;

public:
    explicit RenderSystem(sf::RenderWindow& window) : window(window) {}

    void render(EntityManager& em) {
        // Render background layers first
        auto backgroundEntities = em.getEntitiesWithComponent<BackgroundComponent>("background");
        for (Entity e : backgroundEntities) {
            auto background = em.getComponent<BackgroundComponent>(e, "background");
            auto position = em.getComponent<PositionComponent>(e, "position");

            if (background && position) {
                background->sprite.setPosition(position->x, position->y);
                window.draw(background->sprite);
            }
        }

        auto entities = em.getEntitiesWithComponent<RenderableComponent>("renderable");
        for (Entity e : entities) {
            auto renderable = em.getComponent<RenderableComponent>(e, "renderable");
            auto position = em.getComponent<PositionComponent>(e, "position");
            auto animation = em.getComponent<AnimationComponent>(e, "animation");

            if (renderable && position) {
                if (animation) {
                    // Mise à jour de la frame d'animation
                    renderable->sprite.setTexture(animation->texture);
                    renderable->sprite.setTextureRect(animation->frameRect);
                }
                renderable->sprite.setPosition(position->x, position->y);
                window.draw(renderable->sprite);
            }
        }
    }
};