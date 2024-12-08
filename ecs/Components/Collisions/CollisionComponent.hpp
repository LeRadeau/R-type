#pragma once

#include "../Component.hpp"

#include <vector>
/**
 * @brief Représente les informations relatives aux collisions d'une entité.
 *
 * Ce composant est utilisé par le système de collision pour indiquer si une
 * entité est entrée en collision avec une ou plusieurs autres entités.
 * Il stocke également une liste des entités avec lesquelles la collision a été détectée.
 * 
 * Utilisation :
 * - `hasCollision` : indique si une collision a été détectée (true ou false).
 * - `collidingEntities` : contient les identifiants (`Entity`) des autres entités impliquées dans la collision.
 * 
 * Ce composant peut être utilisé par d'autres systèmes (ex: HealthSystem, DamageSystem)
 * pour réagir aux collisions en appliquant des effets comme une réduction de santé ou
 * des animations.
 */

using Entity = std::size_t;
struct CollisionComponent : public Component {
    bool hasCollision;
    std::vector<Entity> collidingEntities;

    CollisionComponent() : hasCollision(false), collidingEntities() {}
};
