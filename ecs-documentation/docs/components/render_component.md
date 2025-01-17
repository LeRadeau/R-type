# RenderComponent

## Introduction

Le composant `RenderComponent` permet de définir des propriétés visuelles pour une entité à l'aide de formes géométriques. Il est utilisé pour afficher des entités avec des propriétés graphiques telles que la taille, la position et la couleur.

## Attributs

- **shape** : Un objet `sf::RectangleShape` représentant la forme visuelle de l'entité.

### Exemple d'attributs dans le fichier `RenderComponent.hpp` :

```cpp
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

class RenderComponent : public AComponent {
public:
    sf::RectangleShape shape;

    RenderComponent(float width, float height, const sf::Color &color) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(color);
    }
};
```

## Rôle dans l'ECS

`RenderComponent` est utilisé pour :

- Définir une apparence visuelle simple pour une entité.
- Permettre à `RenderSystem` de dessiner l'entité à l'écran.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &entity = entityManager.createEntity();
   entity.addComponent<RenderComponent>(50.0f, 50.0f, sf::Color::Red);
   ```

2. **Modification des propriétés** :
   ```cpp
   auto *render = entity.getComponent<RenderComponent>();
   if (render) {
       render->shape.setFillColor(sf::Color::Blue);
       render->shape.setSize(sf::Vector2f(100.0f, 50.0f));
   }
   ```

## Interactions

- **Avec RenderSystem** : Utilisé pour dessiner la forme définie par le composant à l'écran.
- **Avec PositionComponent** : La position de la forme est synchronisée avec la position de l'entité.
- **Avec HoverComponent** : Peut modifier la couleur ou d'autres propriétés visuelles en réponse à des interactions utilisateur.

## Fonctionnalités supplémentaires

- **Apparence dynamique** : Les propriétés de la forme peuvent être modifiées à la volée pour refléter des changements dans l'état du jeu.
- **Compatibilité avec d'autres composants** : Fonctionne avec d'autres composants graphiques pour enrichir les visuels.

---

`RenderComponent` est un composant simple mais essentiel pour définir l'apparence des entités et les afficher à l'écran.

