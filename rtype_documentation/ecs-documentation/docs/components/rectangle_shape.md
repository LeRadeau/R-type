# RectangleShapeComponent

## Introduction

Le composant `RectangleShapeComponent` est utilisé pour représenter des entités sous forme de rectangles graphiques dans le jeu. Il permet de définir l'apparence et la position d'une entité en utilisant un rectangle SFML.

## Attributs

- **shape** : Un objet `sf::RectangleShape` représentant le rectangle graphique.

### Exemple d'attributs dans le fichier `RectangleShapeComponent.hpp` :

```cpp
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class RectangleShapeComponent : public AComponent {
public:
    sf::RectangleShape shape;

    RectangleShapeComponent(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &color) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(color);
    }
};
```

## Rôle dans l'ECS

`RectangleShapeComponent` est utilisé pour :

- Afficher des entités sous forme de rectangles.
- Définir des propriétés graphiques comme la taille, la couleur et la position.
- Fournir une base pour des interactions telles que le survol ou la sélection.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &button = entityManager.createEntity();
   button.addComponent<RectangleShapeComponent>(sf::Vector2f(150.0f, 50.0f), sf::Vector2f(100.0f, 200.0f), sf::Color::Blue);
   ```

2. **Modification des propriétés** :
   ```cpp
   auto *rectangle = entity.getComponent<RectangleShapeComponent>();
   if (rectangle) {
       rectangle->shape.setFillColor(sf::Color::Green);
       rectangle->shape.setSize(sf::Vector2f(200.0f, 60.0f));
   }
   ```

## Interactions

- **Avec HoverComponent** : Change la couleur du rectangle lorsque la souris le survole.
- **Avec SelectionComponent** : Permet de détecter et d'indiquer si le rectangle est sélectionné.
- **Avec RenderSystem** : Utilisé pour dessiner le rectangle à l'écran.

## Fonctionnalités principales

### Apparence dynamique

Le composant prend en charge la modification dynamique de l'apparence du rectangle, comme le changement de couleur ou de taille en réponse à des événements ou des interactions utilisateur.

---

Le composant `RectangleShapeComponent` offre une représentation graphique simple et efficace pour des entités interactives, telles que des boutons ou des zones de sélection, dans le jeu.

