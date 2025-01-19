# RenderSystem

## Introduction

Le système `RenderSystem` est responsable de l'affichage graphique des entités à l'écran. Il utilise des composants tels que `SpriteComponent`, `TextComponent`, et `RenderComponent` pour dessiner les entités avec leurs attributs visuels respectifs.

## Rôle dans l'ECS

`RenderSystem` est utilisé pour :

- Dessiner les entités et leurs propriétés visuelles.
- Afficher les informations supplémentaires comme les noms d'utilisateur, les scores, et les points de vie.
- Synchroniser les positions et les visuels des entités avec leurs composants associés.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` parcourt toutes les entités, vérifie leurs composants visuels, et les affiche à l'écran.

#### Code extrait de `RenderSystem.cpp`

```cpp
void RenderSystem::update(EntityManager &entityManager, const std::string &playerUsername)
{
    for (auto &entity : entityManager.entities) {
        auto *position = entity->getComponent<PositionComponent>();
        auto *render = entity->getComponent<RenderComponent>();
        auto *username = entity->getComponent<UsernameComponent>();
        auto *health = entity->getComponent<HealthComponent>();
        auto *rectangleShape = entity->getComponent<RectangleShapeComponent>();
        auto *textComponent = entity->getComponent<TextComponent>();
        auto *sprite = entity->getComponent<SpriteComponent>();
        auto *scoreComponent = entity->getComponent<ScoreComponent>();

        if (scoreComponent && username && username->username == playerUsername) {
            sf::Text text;
            text.setFont(font);
            text.setString("SCORE: " + std::to_string(scoreComponent->score));
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            text.setPosition(window.getSize().x / 2.0f, 10);
            window.draw(text);
        }

        if (health && health->health <= 0) {
            continue;
        }

        if (rectangleShape) {
            window.draw(rectangleShape->shape);
        }

        if (textComponent) {
            window.draw(textComponent->data);
        }

        if (position && sprite) {
            sprite->sprite.setPosition(position->position);
            window.draw(sprite->sprite);
        }
    }
}
```

## Interactions

- **Avec SpriteComponent** : Affiche les sprites des entités avec leurs positions et échelles.
- **Avec TextComponent** : Dessine des textes, tels que des noms ou des scores.
- **Avec HealthComponent** : Affiche les points de vie sous forme visuelle.
- **Avec RenderComponent** : Gère les formes géométriques pour les entités.

## Exemples d'Utilisation

1. **Initialisation et mise à jour** :
   ```cpp
   RenderSystem renderSystem(window, "assets/fonts/arial.ttf");
   renderSystem.update(entityManager, "Player1");
   ```

2. **Affichage d'un sprite** :
   ```cpp
   auto *sprite = entity.getComponent<SpriteComponent>();
   if (sprite) {
       sprite->sprite.setPosition(100.0f, 200.0f);
       window.draw(sprite->sprite);
   }
   ```

3. **Affichage de statistiques** :
   ```cpp
   auto *score = entity.getComponent<ScoreComponent>();
   if (score) {
       sf::Text text;
       text.setString("Score: " + std::to_string(score->score));
       window.draw(text);
   }
   ```

## Fonctionnalités supplémentaires

- **Gestion des couches** : Permet d’afficher les entités dans un ordre spécifique.
- **Support multi-composants** : Combine plusieurs composants pour des affichages riches.

---

`RenderSystem` est un système clé pour créer des expériences visuelles immersives, en affichant les entités et leurs attributs de manière cohérente et dynamique.

