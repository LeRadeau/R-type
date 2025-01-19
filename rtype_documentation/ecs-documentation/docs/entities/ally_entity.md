# AllyEntity

## Introduction

`AllyEntity` représente un allié dans le jeu. Ces entités possèdent des composants qui leur permettent d'interagir avec les joueurs, de contribuer aux objectifs du jeu et de suivre des statistiques comme la santé et le score.

## Rôle dans l'ECS

`AllyEntity` est utilisé pour :

- Représenter des alliés ou partenaires contrôlés par l'IA ou d'autres joueurs.
- Gérer des statistiques comme la santé et le score.
- Synchroniser les données entre le serveur et le client pour une expérience multijoueur.

## Attributs

Les entités `AllyEntity` incluent les composants suivants :

- **PositionComponent** : Définit la position de l'allié.
- **SpriteComponent** : Affiche un sprite graphique pour représenter l'allié visuellement.
- **UsernameComponent** : Identifie l'allié avec un nom unique.
- **TextComponent** : Affiche le nom d'utilisateur ou des informations supplémentaires à côté de l'entité.
- **ScoreComponent** : Suivi du score individuel de l'allié.
- **HealthComponent** : Gère la santé de l'allié.

### Exemple de création

#### Code extrait de `AllyEntity.cpp`

```cpp
Entity &AllyEntity::createAllyEntity(EntityManager &entityManager, float x, float y, int health, int score,
    const std::string &username, const sf::Font &font)
{
    Entity &ally = entityManager.createEntity();
    ally.addComponent<PositionComponent>(x, y);
    ally.addComponent<SpriteComponent>("assets/Ally64x64.png", sf::Vector2f(1, 1));
    ally.addComponent<UsernameComponent>(username);
    auto &text = ally.addComponent<TextComponent>(username, font, sf::Vector2f(x, y), sf::Color::White);
    text.data.setCharacterSize(24);
    ally.addComponent<ScoreComponent>().score = score;
    ally.addComponent<HealthComponent>(health);
    return ally;
}
```

## Fonctionnalités principales

### Synchronisation des statistiques

Suivi de la santé et du score pour chaque allié :

```cpp
auto *health = ally.getComponent<HealthComponent>();
if (health) {
    health->health -= 10;
}

auto *score = ally.getComponent<ScoreComponent>();
if (score) {
    score->score += 5;
}
```

## Interactions

- **Avec RenderSystem** : Affiche les alliés et leurs informations sur l'écran.
- **Avec MessageSystem** : Met à jour la position et les statistiques en fonction des données reçues du serveur.
- **Avec HealthComponent** : Suivi et gestion de l'état de santé des alliés.
- **Avec ScoreComponent** : Mise à jour et affichage des scores.

## Exemples d'Utilisation

1. **Création d'un allié** :
   ```cpp
   AllyEntity::createAllyEntity(entityManager, 100.0f, 200.0f, 100, 0, "Ally1", font);
   ```

2. **Mise à jour de la santé** :
   ```cpp
   auto *health = ally.getComponent<HealthComponent>();
   if (health) {
       health->health -= 15;
   }
   ```

3. **Affichage du score** :
   ```cpp
   auto *score = ally.getComponent<ScoreComponent>();
   if (score) {
       std::cout << "Score de l'allié : " << score->score << std::endl;
   }
   ```

---

`AllyEntity` est une entité essentielle pour représenter les alliés dans le jeu, offrant une gestion complète des statistiques et des interactions multijoueurs.

