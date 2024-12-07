#pragma once

// Entities Components
#include "../../Entities/EntityManager.hpp"

// Transforms Components
#include "../../Components/Transforms/PositionComponent.hpp"

// Physics Components
#include "../../Components/Physics/BoundingBoxComponent.hpp"

// General Components
#include "../../Components/General/HealthComponent.hpp"

#include <iostream>

class CollisionSystem {
public:
    void update(EntityManager& em, float dt) {
        auto entities = em.getEntitiesWithComponent<BoundingBoxComponent>("boundingBox");

        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = 0; j < entities.size(); ++j) {
                if (i == j) continue;

                auto tagA = em.getComponent<TagComponent>(entities[i], "tag");
                auto tagB = em.getComponent<TagComponent>(entities[j], "tag");
                auto boxA = em.getComponent<BoundingBoxComponent>(entities[i], "boundingBox");
                auto posA = em.getComponent<PositionComponent>(entities[i], "position");
                auto boxB = em.getComponent<BoundingBoxComponent>(entities[j], "boundingBox");
                auto posB = em.getComponent<PositionComponent>(entities[j], "position");
                auto cooldownA = em.getComponent<TimeCooldownComponent>(entities[i], "cooldown");

                if (tagA && tagB && boxA && posA && boxB && posB && cooldownA) {
                    // Vérifie si le cooldown est actif
                    if (cooldownA->elapsedTime < cooldownA->cooldownTime) {
                        cooldownA->elapsedTime += dt;
                        continue;
                    }

                    // Collision entre player et enemy
                    if (tagA->tag == "player" && tagB->tag == "enemy") {
                        if (posA->x < posB->x + boxB->width &&
                            posA->x + boxA->width > posB->x &&
                            posA->y < posB->y + boxB->height &&
                            posA->y + boxA->height > posB->y) {
                            std::cout << "Collision detected between entities " << entities[i] << " and " << entities[j] << std::endl;
                            auto healthA = em.getComponent<HealthComponent>(entities[i], "health");
                            if (healthA) {
                                healthA->health -= 25;
                                cooldownA->elapsedTime = 0; // Réinitialise le cooldown
                                std::cout << "Player health: " << healthA->health << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
};
