#pragma once

// Entities Components
#include "../../Entities/EntityManager.hpp"

// Inputs Components
#include "../../Components/Inputs/InputComponent.hpp"

// Transforms Components
#include "../../Components/Transforms/PositionComponent.hpp"

// Physics Components
#include "../../Components/Physics/VelocityComponent.hpp"
#include "../../Components/Physics/DestinationComponent.hpp"

#include <cmath>


class MovementSystem {
public:
    void update(EntityManager& em, float dt) {
        // Process entities with InputComponent for movement
        auto inputEntities = em.getEntitiesWithComponent<InputComponent>("input");
        for (Entity e : inputEntities) {
            auto position = em.getComponent<PositionComponent>(e, "position");
            auto velocity = em.getComponent<VelocityComponent>(e, "velocity");
            auto input = em.getComponent<InputComponent>(e, "input");

            if (position && velocity) {
                velocity->vx = 0.0f;
                velocity->vy = 0.0f;
                if (input->moveUp)      velocity->vy = -velocity->speed;
                if (input->moveDown)    velocity->vy = velocity->speed;
                if (input->moveLeft)    velocity->vx = -velocity->speed;
                if (input->moveRight)   velocity->vx = velocity->speed;

                if (velocity->vx != 0 && velocity->vy != 0) {
                    velocity->vx *= 0.7071f;
                    velocity->vy *= 0.7071f;
                }
                position->x += velocity->vx * dt;
                position->y += velocity->vy * dt;
            }
        }

        // Process entities with DestinationComponent for movement
        auto destinationEntities = em.getEntitiesWithComponent<DestinationComponent>("destination");
        for (Entity e : destinationEntities) {
            auto position = em.getComponent<PositionComponent>(e, "position");
            auto velocity = em.getComponent<VelocityComponent>(e, "velocity");
            auto destination = em.getComponent<DestinationComponent>(e, "destination");

            if (position && velocity && destination) {
                if (destination->todes) {
                    float dx = destination->desx - position->x;
                    float dy = destination->desy - position->y;
                    float angle = std::atan2(dy, dx);
                    velocity->vx = velocity->speed * std::cos(angle);
                    velocity->vy = velocity->speed * std::sin(angle);

                    if (std::abs(dx) < 5 && std::abs(dy) < 5) {
                        destination->todes = false;
                    }
                }
                else {
                    float dx = destination->depx - position->x;
                    float dy = destination->depy - position->y;
                    float angle = std::atan2(dy, dx);
                    velocity->vx = velocity->speed * std::cos(angle);
                    velocity->vy = velocity->speed * std::sin(angle);

                    if (std::abs(dx) < 10 && std::abs(dy) < 10) {
                        destination->todes = true;
                    }
                }
                position->x += velocity->vx * dt;
                position->y += velocity->vy * dt;
            }
        }
    }
};