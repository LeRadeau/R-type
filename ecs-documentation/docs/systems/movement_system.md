# MovementSystem

## Introduction

Le système `MovementSystem` est responsable de la mise à jour des positions des entités en fonction de leur vitesse et des entrées utilisateur. Il joue un rôle clé dans le déplacement des entités dans le jeu.

## Rôle dans l'ECS

`MovementSystem` a pour rôle de :

- Calculer les nouvelles positions des entités en fonction de leur `VelocityComponent`.
- Traiter les entrées utilisateur pour influencer le mouvement des entités.
- Supprimer ou réinitialiser les entités en dehors des limites du jeu.

## Fonctionnalités principales

### Méthode `update`

Cette méthode parcourt toutes les entités ayant des composants pertinents (comme `VelocityComponent` et `PositionComponent`) et met à jour leurs positions.

#### Exemple de code

```cpp
void MovementSystem::update(EntityManager &entityManager, NetworkManager &networkManager, float deltaTime, bool focus) {
    static float timer = 0.0f;
    timer += deltaTime;

    for (auto &entity : entityManager.entities) {
        auto *velocity = entity->getComponent<VelocityComponent>();
        auto *position = entity->getComponent<PositionComponent>();

        if (velocity && position) {
            position->position.x += velocity->velocity.x * deltaTime;
            position->position.y += velocity->velocity.y * deltaTime;

            // Gestion des entités en dehors des limites du jeu
            if (position->position.x < 0 || position->position.x > 1920 ||
                position->position.y < 0 || position->position.y > 1080) {
                entityManager.markForDeletion(entity->getId());
            }
        }
    }
    entityManager.destroyMarkedEntities();
}
```

### Gestion des entrées utilisateur

Le système traite également les entrées utilisateur pour influencer la vitesse ou la direction :

```cpp
if (input && position && input->moveLeft) {
    position->position.x -= 200.0f * deltaTime;
}
```

## Exemples d'Utilisation

1. **Initialisation et appel** :
   ```cpp
   MovementSystem movementSystem;
   movementSystem.update(entityManager, networkManager, 0.016f, true);
   ```

2. **Suppression des entités hors limites** :
   ```cpp
   if (position->position.x < 0 || position->position.x > 1920) {
       entityManager.markForDeletion(entity->getId());
   }
   ```

## Interactions

- **Avec PositionComponent** : Met à jour la position en fonction des calculs du système.
- **Avec VelocityComponent** : Utilise la vitesse pour déterminer les déplacements.
- **Avec InputComponent** : Permet aux utilisateurs d'influencer les mouvements des entités.
- **Avec NetworkManager** : Synchronise les mouvements des entités dans un environnement multijoueur.

---

Le système `MovementSystem` est essentiel pour gérer les déplacements des entités, en combinant la logique des composants de position, de vitesse et des entrées utilisateur.

