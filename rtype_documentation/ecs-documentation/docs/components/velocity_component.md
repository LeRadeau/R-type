# VelocityComponent

## Introduction

Le composant `VelocityComponent` est utilisé pour représenter la vitesse et la direction d'une entité dans le jeu. Il permet de gérer les déplacements en combinant les informations de position et de vitesse.

## Attributs

- **velocity** : Un vecteur 2D (souvent `sf::Vector2f`) représentant la vitesse sur les axes X et Y.

### Exemple d'attributs dans le fichier `VelocityComponent.hpp` :

```cpp
#include <SFML/System/Vector2.hpp>

class VelocityComponent : public AComponent {
public:
    sf::Vector2f velocity;

    VelocityComponent(float x, float y) : velocity(x, y) {}
};
```

## Rôle dans l'ECS

`VelocityComponent` est utilisé pour :

- Définir la vitesse et la direction des entités en mouvement.
- Permettre aux systèmes comme `MovementSystem` d'effectuer des mises à jour précises de la position des entités.
- Implémenter des mécaniques de jeu telles que des projectiles, des ennemis en mouvement ou des véhicules.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &entity = entityManager.createEntity();
   entity.addComponent<VelocityComponent>(100.0f, 0.0f);
   ```

2. **Mise à jour de la vitesse** :
   ```cpp
   auto *velocity = entity->getComponent<VelocityComponent>();
   if (velocity) {
       velocity->velocity.x += 10.0f;
       velocity->velocity.y -= 5.0f;
   }
   ```

3. **Utilisation dans un système** :
   Dans un système comme `MovementSystem`, la vitesse est utilisée pour mettre à jour la position :
   ```cpp
   void MovementSystem::update(EntityManager &entityManager, float deltaTime) {
       for (auto &entity : entityManager.entities) {
           auto *velocity = entity->getComponent<VelocityComponent>();
           auto *position = entity->getComponent<PositionComponent>();
           if (velocity && position) {
               position->position.x += velocity->velocity.x * deltaTime;
               position->position.y += velocity->velocity.y * deltaTime;
           }
       }
   }
   ```

## Interactions

- **Avec PositionComponent** : Combine les informations de vitesse pour mettre à jour la position des entités.
- **Avec InputComponent** : Modifie la direction et la vitesse en fonction des commandes utilisateur.
- **Avec CollisionSystem** : Peut inverser ou ajuster la vitesse lors de collisions.

---

Le composant `VelocityComponent` est essentiel pour implémenter des mécaniques de mouvement réalistes et dynamiques dans le jeu.

