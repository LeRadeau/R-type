# PositionComponent

## Introduction

Le composant `PositionComponent` est utilisé pour stocker et gérer la position spatiale d'une entité dans le monde du jeu. Il est fondamental pour toute entité ayant une représentation physique ou graphique.

## Attributs

- **position** : Un vecteur 2D (souvent représenté par `sf::Vector2f` ou une structure équivalente) contenant les coordonnées X et Y de l'entité.

### Exemple d'attributs dans le fichier `PositionComponent.hpp` :

```cpp
#include <SFML/System/Vector2.hpp>

class PositionComponent : public AComponent {
public:
    sf::Vector2f position;

    PositionComponent(float x, float y) : position(x, y) {}
};
```

## Rôle dans l'ECS

`PositionComponent` est utilisé pour :

- Définir et suivre la position d'une entité sur la carte.
- Permettre aux systèmes comme `MovementSystem` et `RenderSystem` d'accéder à la position de l'entité.
- Servir de base pour des calculs tels que les collisions ou les distances entre entités.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &entity = entityManager.createEntity();
   entity.addComponent<PositionComponent>(100.0f, 200.0f);
   ```

2. **Mise à jour de la position** :
   Utilisé dans un système tel que `MovementSystem` :
   ```cpp
   auto *position = entity->getComponent<PositionComponent>();
   if (position) {
       position->position.x += 10.0f;
       position->position.y += 5.0f;
   }
   ```

3. **Accès à la position** :
   Dans un système de rendu ou de collision :
   ```cpp
   auto *position = entity->getComponent<PositionComponent>();
   if (position) {
       std::cout << "Position actuelle : (" << position->position.x << ", " << position->position.y << ")" << std::endl;
   }
   ```

## Interactions

- **Avec MovementSystem** : Permet la mise à jour des coordonnées en fonction des vitesses et des entrées utilisateur.
- **Avec RenderSystem** : Utilisé pour déterminer où dessiner les entités sur l'écran.
- **Avec CollisionSystem** : Évalué pour détecter les chevauchements ou interactions entre entités.

---

Le composant `PositionComponent` est un élément essentiel dans tout jeu, fournissant les coordonnées nécessaires pour toutes les opérations spatiales.

