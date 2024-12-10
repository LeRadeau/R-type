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
        auto inputEntities = em.getEntitiesWithComponent<InputComponent>("input");
        for (Entity e : inputEntities) {
            auto position = em.getComponent<PositionComponent>(e, "position");
            auto velocity = em.getComponent<VelocityComponent>(e, "velocity");
            auto input = em.getComponent<InputComponent>(e, "input");

            if (position && velocity && input) {
                velocity->vx = 0.0f;
                velocity->vy = 0.0f;

                if (input->keyStates[sf::Keyboard::Up]) velocity->vy = -velocity->speed; // Haut
                if (input->keyStates[sf::Keyboard::Down]) velocity->vy = velocity->speed;  // Bas
                if (input->keyStates[sf::Keyboard::Left]) velocity->vx = -velocity->speed; // Gauche
                if (input->keyStates[sf::Keyboard::Right]) velocity->vx = velocity->speed;  // Droite

                if (velocity->vx != 0 && velocity->vy != 0) {
                    velocity->vx *= 0.7071f;
                    velocity->vy *= 0.7071f;
                }

                position->x += velocity->vx * dt;
                position->y += velocity->vy * dt;
            }
        }

        auto destinationEntities = em.getEntitiesWithComponent<DestinationComponent>("destination");
        for (Entity e : destinationEntities) {
            auto position = em.getComponent<PositionComponent>(e, "position");
            auto velocity = em.getComponent<VelocityComponent>(e, "velocity");
            auto destination = em.getComponent<DestinationComponent>(e, "destination");

            if (position && velocity && destination) {
                float dx, dy, angle;

                if (destination->todes) {
                    dx = destination->desx - position->x;
                    dy = destination->desy - position->y;
                } else {
                    dx = destination->depx - position->x;
                    dy = destination->depy - position->y;
                }

                angle = std::atan2(dy, dx);
                velocity->vx = velocity->speed * std::cos(angle);
                velocity->vy = velocity->speed * std::sin(angle);

                if (std::hypot(dx, dy) < 5.0f) {
                    destination->todes = !destination->todes;
                    velocity->vx = 0.0f;
                    velocity->vy = 0.0f;
                }

                position->x += velocity->vx * dt;
                position->y += velocity->vy * dt;
            }
        }
    }
};
