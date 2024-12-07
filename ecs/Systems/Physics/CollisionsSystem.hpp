#pragma once

// Entities Components
#include "../../Entities/EntityManager.hpp"

// Transforms Components
#include "../../Components/Transforms/PositionComponent.hpp"

// Physics Components
#include "../../Components/Physics/BoundingBoxComponent.hpp"

#include <iostream>

class CollisionSystem {
public:
    void update(EntityManager& em) {
        auto entities = em.getEntitiesWithComponent<BoundingBoxComponent>("boundingBox");
        
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                auto boxA = em.getComponent<BoundingBoxComponent>(entities[i], "boundingBox");
                auto posA = em.getComponent<PositionComponent>(entities[i], "position");
                auto boxB = em.getComponent<BoundingBoxComponent>(entities[j], "boundingBox");
                auto posB = em.getComponent<PositionComponent>(entities[j], "position");

                if (boxA && posA && boxB && posB) {
                    // Vérifie si les boîtes de collision se chevauchent
                    if (posA->x < posB->x + boxB->width &&
                        posA->x + boxA->width > posB->x &&
                        posA->y < posB->y + boxB->height &&
                        posA->y + boxA->height > posB->y) {
                        std::cout << "Collision detected between entities " << entities[i] << " and " << entities[j] << std::endl;
                    }
                }
            }
        }
    }
};