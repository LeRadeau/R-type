# RenderSystem

## Introduction

Le système `RenderSystem` est chargé d'afficher les entités sur l'écran. Il utilise les composants graphiques comme `RenderComponent` et `SpriteComponent` pour dessiner les entités en fonction de leurs positions et de leurs propriétés visuelles.

## Rôle dans l'ECS

`RenderSystem` a pour rôle de :

- Dessiner les entités en utilisant leurs composants graphiques.
- Afficher les informations supplémentaires, telles que les noms d'utilisateur ou les barres de vie.
- Fournir une interface pour gérer les mises à jour visuelles en temps réel.

## Fonctionnalités principales

### Méthode `update`

Cette méthode parcourt toutes les entités ayant des composants graphiques pertinents et les dessine à l'écran.

#### Exemple de code

```cpp
void RenderSystem::update(EntityManager &entityManager) {
    for (auto &entity : entityManager.entities) {
        auto *render = entity->getComponent<RenderComponent>();
        auto *position = entity->getComponent<PositionComponent>();

        if (render && position) {
            render->shape.setPosition(position->position);
            window.draw(render->shape);
        }

        auto *username = entity->getComponent<UsernameComponent>();
        if (username) {
            sf::Text text;
            text.setFont(font);
            text.setString(username->username);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(position->position.x, position->position.y - 30);
            window.draw(text);
        }
    }
}
```

### Affichage des barres d'information

Le système peut également afficher des éléments tels que les barres de vie ou des statistiques globales :

```cpp
void RenderSystem::displayBarInfos(EntityManager &entityManager) {
    sf::RectangleShape bar(sf::Vector2f(window.getSize().x, 70));
    bar.setFillColor(sf::Color::Blue);
    window.draw(bar);
}
```

## Exemples d'Utilisation

1. **Initialisation et appel** :
   ```cpp
   sf::RenderWindow window(sf::VideoMode(1920, 1080), "R-Type");
   RenderSystem renderSystem(window);
   renderSystem.update(entityManager);
   ```

2. **Affichage des noms d'utilisateur** :
   ```cpp
   auto *username = entity->getComponent<UsernameComponent>();
   if (username) {
       sf::Text text;
       text.setFont(font);
       text.setString(username->username);
       text.setCharacterSize(24);
       window.draw(text);
   }
   ```

## Interactions

- **Avec RenderComponent** : Utilisé pour définir les propriétés graphiques des entités.
- **Avec PositionComponent** : Permet de dessiner les entités à la position correcte.
- **Avec UsernameComponent** : Affiche les noms d'utilisateur au-dessus des entités.
- **Avec HealthComponent** : Ajoute des indicateurs graphiques pour les points de vie.

---

Le système `RenderSystem` est essentiel pour offrir une représentation visuelle des entités et des interactions dans le jeu, en s'appuyant sur les composants graphiques pour afficher une expérience utilisateur cohérente et immersive.

