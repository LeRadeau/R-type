# RenderComponent

## Introduction

Le composant `RenderComponent` gère les informations nécessaires pour afficher une entité à l'écran. Il contient des données sur la forme, la couleur et d'autres propriétés graphiques.

## Attributs

- **shape** : Un objet graphique (par exemple, `sf::RectangleShape` ou `sf::CircleShape`) représentant l'apparence de l'entité.

### Exemple d'attributs dans le fichier `RenderComponent.hpp` :

```cpp
#include <SFML/Graphics/RectangleShape.hpp>

class RenderComponent : public AComponent {
public:
    sf::RectangleShape shape;

    RenderComponent(float width, float height, sf::Color color) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(color);
    }
};
```

## Rôle dans l'ECS

`RenderComponent` est utilisé pour :

- Définir l'apparence visuelle d'une entité.
- Fournir les données nécessaires pour le rendu graphique dans des systèmes comme `RenderSystem`.
- Permettre des effets visuels tels que le changement de couleur ou de taille en fonction des états du jeu.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &entity = entityManager.createEntity();
   entity.addComponent<RenderComponent>(50.0f, 50.0f, sf::Color::Red);
   ```

2. **Mise à jour de l'apparence** :
   Dans un système, on peut modifier les propriétés du composant :
   ```cpp
   auto *render = entity->getComponent<RenderComponent>();
   if (render) {
       render->shape.setFillColor(sf::Color::Blue);
   }
   ```

3. **Rendu dans un système** :
   Utilisé par `RenderSystem` pour dessiner l'entité :
   ```cpp
   void RenderSystem::update(EntityManager &entityManager) {
       for (auto &entity : entityManager.entities) {
           auto *render = entity->getComponent<RenderComponent>();
           auto *position = entity->getComponent<PositionComponent>();
           if (render && position) {
               render->shape.setPosition(position->position);
               window.draw(render->shape);
           }
       }
   }
   ```

## Interactions

- **Avec PositionComponent** : Associe la position de l'entité à son rendu graphique.
- **Avec HealthComponent** : Permet d'afficher des indicateurs visuels (par exemple, une barre de vie).
- **Avec InputComponent** : Peut modifier l'apparence en fonction des actions de l'utilisateur.

---

Le composant `RenderComponent` est essentiel pour donner vie aux entités du jeu en fournissant les données nécessaires pour les afficher à l'écran.

