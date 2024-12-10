#pragma once

// Entities Components
#include "../../Entities/EntityManager.hpp"

// Inputs Components
#include "../../Components/Inputs/InputComponent.hpp"

// Transforms Components
#include "../../Components/Transforms/PositionComponent.hpp"

// Renders Components
#include "../../Components/Renders/BackgroundComponent.hpp"

/**
 * ParallaxSystem :
 * 
 * Ce système gère l'effet de parallaxe pour les entités avec un BackgroundComponent 
 * et un PositionComponent, en déplaçant les calques d'arrière-plan à différentes vitesses.
 * 
 * Fonctionnalités :
 * - Déplace les calques selon parallaxSpeed et le temps écoulé (dt).
 * - Réinitialise la position lorsqu'un calque dépasse une limite pour une animation continue.
 * 
 * Utilisation :
 * - Ajoute un effet de profondeur visuelle dans les jeux avec des arrière-plans dynamiques.
 */

class ParallaxSystem {
public:
    void update(EntityManager& em, float dt) {
        auto entities = em.getEntitiesWithComponent<BackgroundComponent>("background");
        for (Entity e : entities) {
            auto background = em.getComponent<BackgroundComponent>(e, "background");
            auto position = em.getComponent<PositionComponent>(e, "position");
            
            if (background && position) {
                position->x -= background->parallaxSpeed * dt;

                if (position->x < -1920) position->x = 0;
            }
        }
    }
};
