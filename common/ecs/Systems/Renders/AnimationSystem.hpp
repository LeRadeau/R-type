#pragma once

// Entities Components
#include "../../Entities/EntityManager.hpp"

//Renders Components
#include "../../Components/Renders/AnimationComponent.hpp"

// Transforms Components
#include "../../Components/Transforms/PositionComponent.hpp"


#include <SFML/Graphics.hpp>

class AnimationSystem {
public:
    void update(EntityManager& em, float dt) {
        auto entities = em.getEntitiesWithComponent<AnimationComponent>("animation");

        for (Entity entity : entities) {
            auto animation = em.getComponent<AnimationComponent>(entity, "animation");
            auto position = em.getComponent<PositionComponent>(entity, "position"); // Pour le positionnement (facultatif)

            if (animation) {
                // Met à jour le temps écoulé
                animation->elapsedTime += dt;

                // Change de frame si nécessaire
                if (animation->elapsedTime >= animation->timePerFrame) {
                    animation->elapsedTime -= animation->timePerFrame; // Réinitialise le temps pour la prochaine frame
                    animation->currentFrame = (animation->currentFrame + 1) % animation->frameCount;

                    // Met à jour le rectangle de la frame actuelle
                    animation->frameRect.left = animation->currentFrame * animation->frameRect.width;
                }
            }
        }
    }
};