#pragma once

// Entities Components
#include "../../Entities/EntityManager.hpp"

// Transforms Components
#include "../../Components/Transforms/PositionComponent.hpp"

// Physics Components
#include "../../Components/Physics/BoundingBoxComponent.hpp"

#include <vector>
//axe d' amélioration moins de redondance des fonctions getComponent
class CollisionSystem
{
    public:
        void update(EntityManager &em, float dt)
        {
            auto collisionEntities = em.getEntitiesWithComponent<CollisionComponent>("collision");
            for (Entity e : collisionEntities) {
                auto col = em.getComponent<CollisionComponent>(e, "collision");
                if (col) {
                    col->hasCollision = false;
                    col->collidingEntities.clear();
                }
            }
            for (Entity EntityA : collisionEntities) {
                auto colA = em.getComponent<CollisionComponent>(EntityA, "collision");
                auto boxA = em.getComponent<BoundingBoxComponent>(EntityA, "boundingBox");
                auto posA = em.getComponent<PositionComponent>(EntityA, "position");
                if (colA && boxA && posA) {
                    for (Entity EntityB : collisionEntities) {
                        if (EntityB == EntityA) {
                            continue;
                        }
                        auto colB = em.getComponent<CollisionComponent>(EntityB, "collision");
                        auto boxB = em.getComponent<BoundingBoxComponent>(EntityB, "boundingBox");
                        auto posB = em.getComponent<PositionComponent>(EntityB, "position");
                        if (colB && boxB && posB) {
                            if (posA->x < posB->x + boxB->width &&
                                posA->x + boxA->width > posB->x &&
                                posA->y < posB->y + boxB->height &&
                                posA->y + boxA->height > posB->y) {
                                    colA->hasCollision = true;
                                    colA->collidingEntities.push_back(EntityB);
                                    colB->hasCollision = true;
                                    colB->collidingEntities.push_back(EntityA);
                                    std::cout << "Entity " << EntityA << " collided with: " << EntityB << std::endl;

                            }
                        }
                    }
                }
            }
        }
    private:
};
