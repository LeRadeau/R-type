#pragma once

#include "../entitiesManager.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"

class MovementSystem {
public:
    void update(EntityManager& entityManager) {
        for (auto& entity : entityManager.entities) {
            auto* input = entity->getComponent<InputComponent>();
            auto* position = entity->getComponent<PositionComponent>();

            if (input && position) {
                if (input->moveLeft) 
                    position->position.x -= 1.0f;
                if (input->moveRight)
                    position->position.x += 1.0f;
                if (input->moveUp)
                    position->position.y -= 1.0f;
                if (input->moveDown)
                    position->position.y += 1.0f;
            }
        }
    }
};