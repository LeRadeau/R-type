# HoverSystem

## Introduction

`HoverSystem` gère les interactions visuelles liées au survol de la souris sur les entités. Il modifie l'apparence des entités ayant un composant `HoverComponent` en fonction de la position actuelle de la souris.

## Rôle dans l'ECS

`HoverSystem` est utilisé pour :

- Détecter si la souris survole une entité.
- Modifier l'apparence visuelle des entités pour fournir un retour utilisateur.
- Faciliter les interactions intuitives avec des entités interactives comme des boutons ou des éléments graphiques.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` parcourt toutes les entités et applique les couleurs définies dans le `HoverComponent` en fonction de la position de la souris.

#### Code extrait de `HoverSystem.cpp` :

```cpp
void HoverSystem::update(EntityManager &entityManager, float x, float y)
{
    for (auto &i : entityManager.entities) {
        auto *hoverComponent = i->getComponent<HoverComponent>();
        auto *spriteComponent = i->getComponent<SpriteComponent>();
        auto *rectangleShapeComponent = i->getComponent<RectangleShapeComponent>();
        
        if (hoverComponent && spriteComponent) {
            if (spriteComponent->sprite.getGlobalBounds().contains(x, y)) {
                spriteComponent->sprite.setColor(hoverComponent->hoverColor);
            } else {
                spriteComponent->sprite.setColor(hoverComponent->defaultColor);
            }
        }
        if (hoverComponent && rectangleShapeComponent) {
            if (rectangleShapeComponent->shape.getGlobalBounds().contains(x, y)) {
                rectangleShapeComponent->shape.setFillColor(hoverComponent->hoverColor);
            } else {
                rectangleShapeComponent->shape.setFillColor(hoverComponent->defaultColor);
            }
        }
    }
}
```

## Interactions

- **Avec HoverComponent** : Utilise les couleurs définies pour le survol et l'état par défaut.
- **Avec SpriteComponent** : Change la couleur des sprites en fonction de la position de la souris.
- **Avec RectangleShapeComponent** : Modifie la couleur des formes rectangulaires selon le survol.

## Exemples d'Utilisation

1. **Initialisation et utilisation** :
   ```cpp
   sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
   hoverSystem.update(entityManager, static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
   ```

2. **Ajout d'un HoverComponent** :
   ```cpp
   Entity &button = entityManager.createEntity();
   button.addComponent<HoverComponent>(sf::Color::White, sf::Color::Blue);
   ```

## Fonctionnalités supplémentaires

- **Retour visuel immédiat** : Améliore l'expérience utilisateur en fournissant un retour intuitif lors du survol.
- **Gestion flexible** : Compatible avec différents types de composants visuels (sprites, formes).

---

`HoverSystem` est un système essentiel pour les interfaces utilisateur et les interactions graphiques, offrant une expérience fluide et immersive.

