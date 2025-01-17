# MovementSystem

## Introduction

Le système `MovementSystem` est chargé de mettre à jour les positions des entités en fonction de leurs composants `InputComponent`, `VelocityComponent` et autres propriétés associées. Il gère également les interactions comme les tirs ou les collisions hors écran.

## Rôle dans l'ECS

`MovementSystem` est utilisé pour :

- Déplacer les entités basées sur leurs vitesses et directions.
- Réagir aux entrées utilisateur pour contrôler les déplacements.
- Supprimer les entités qui sortent des limites définies.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` parcourt toutes les entités et applique les mises à jour de position en fonction des composants pertinents.

#### Code extrait de `MovementSystem.cpp`

```cpp
void MovementSystem::update(EntityManager &entityManager, NetworkManager &networkManager, float deltaTime, bool focus)
{
    for (auto &entity : entityManager.entities) {
        auto *input = entity->getComponent<InputComponent>();
        auto *position = entity->getComponent<PositionComponent>();
        auto *velocity = entity->getComponent<VelocityComponent>();

        if (input && position && focus) {
            if (input->moveLeft) position->position.x -= 200.0f * deltaTime;
            if (input->moveRight) position->position.x += 200.0f * deltaTime;
            if (input->moveUp) position->position.y -= 200.0f * deltaTime;
            if (input->moveDown) position->position.y += 200.0f * deltaTime;
        }

        if (position && velocity) {
            position->position.x += velocity->velocity.x * deltaTime;
            position->position.y += velocity->velocity.y * deltaTime;
        }

        if (position && (position->position.y < 0 || position->position.x < 0 ||
                         position->position.x > 1920 || position->position.y > 1080)) {
            entityManager.markForDeletion(entity->getId());
        }
    }
    entityManager.destroyMarkedEntities();
}
```

## Interactions

- **Avec InputComponent** : Utilise les entrées utilisateur pour ajuster les positions.
- **Avec VelocityComponent** : Applique des vitesses pour calculer les déplacements.
- **Avec EntityManager** : Gère la suppression des entités hors limites.
- **Avec NetworkManager** : Synchronise les positions des entités dans un environnement multijoueur.

## Exemples d'Utilisation

1. **Initialisation et mise à jour** :
   ```cpp
   movementSystem.update(entityManager, networkManager, deltaTime, true);
   ```

2. **Ajout d'un composant de vitesse** :
   ```cpp
   entity.addComponent<VelocityComponent>(sf::Vector2f(100.0f, 0.0f));
   ```

3. **Suppression des entités hors écran** :
   ```cpp
   if (position->position.x > 1920) {
       entityManager.markForDeletion(entity->getId());
   }
   ```

## Fonctionnalités supplémentaires

- **Gestion des collisions hors limites** : Supprime automatiquement les entités sortant de l'écran.
- **Prise en charge multijoueur** : Synchronise les déplacements des joueurs avec le serveur.

---

`MovementSystem` est essentiel pour gérer les déplacements des entités dans le jeu, en combinant des interactions utilisateur, des vitesses et des mises à jour dynamiques.

