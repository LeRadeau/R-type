# HoverComponent

## Introduction

Le composant `HoverComponent` est utilisé pour gérer les interactions visuelles lorsque la souris survole une entité. Il permet de changer l'apparence d'une entité, telle que sa couleur, lorsqu'elle est survolée.

## Attributs

- **defaultColor** : La couleur par défaut de l'entité.
- **hoverColor** : La couleur affichée lorsque la souris survole l'entité.

### Exemple d'attributs dans le fichier `HoverComponent.hpp` :

```cpp
#include <SFML/Graphics/Color.hpp>

class HoverComponent : public AComponent {
public:
    sf::Color defaultColor;
    sf::Color hoverColor;

    HoverComponent(const sf::Color &defaultColor, const sf::Color &hoverColor)
        : defaultColor(defaultColor), hoverColor(hoverColor) {}
};
```

## Rôle dans l'ECS

`HoverComponent` est utilisé pour :

- Détecter les interactions de la souris avec une entité.
- Modifier visuellement une entité pour indiquer qu'elle est survolée.
- Améliorer l'expérience utilisateur en offrant des retours visuels immédiats.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &button = entityManager.createEntity();
   button.addComponent<HoverComponent>(sf::Color::White, sf::Color::Red);
   ```

2. **Modification des couleurs** :
   ```cpp
   auto *hover = entity.getComponent<HoverComponent>();
   if (hover) {
       hover->defaultColor = sf::Color::Blue;
       hover->hoverColor = sf::Color::Green;
   }
   ```

## Interactions

- **Avec HoverSystem** : Le système applique les changements de couleur définis dans le composant en fonction de la position de la souris.
- **Avec RectangleShapeComponent et SpriteComponent** : Les couleurs définies dans `HoverComponent` sont appliquées aux formes ou sprites associés.

## Fonctionnalités principales

### Gestion du survol

Le composant travaille en tandem avec `HoverSystem` pour détecter si la souris est au-dessus d'une entité. En fonction de cette détection, il applique soit la couleur par défaut, soit la couleur de survol.

---

Le composant `HoverComponent` ajoute une couche d'interactivité visuelle, essentielle pour des éléments tels que les boutons ou les entités interactives dans le jeu.

