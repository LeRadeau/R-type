# PlayerEntity

## Introduction

`PlayerEntity` représente le joueur dans le jeu. Cette entité contient des composants essentiels pour gérer les interactions, l'apparence, la santé et les actions spécifiques du joueur.

## Rôle dans l'ECS

`PlayerEntity` est utilisé pour :

- Représenter le joueur avec une position et un sprite.
- Gérer les interactions utilisateur à travers des entrées clavier.
- Jouer des sons associés au joueur.
- Suivre les statistiques comme le score et la santé.

## Attributs

Les entités `PlayerEntity` incluent les composants suivants :

- **PositionComponent** : Définit la position initiale du joueur.
- **SpriteComponent** : Représente graphiquement le joueur avec un sprite.
- **InputComponent** : Capture les entrées utilisateur pour contrôler le joueur.
- **UsernameComponent** : Identifie le joueur avec un nom unique.
- **SoundComponent** : Joue des sons associés au joueur.
- **ScoreComponent** : Suit les points marqués par le joueur.
- **HealthComponent** : Gère la santé du joueur.

### Exemple de création

#### Code extrait de `PlayerEntity.cpp`

```cpp
PlayerEntity::PlayerEntity(EntityManager &entityManager, const std::string &username)
    : entity_(entityManager.createEntity())
{
    entity_.addComponent<PositionComponent>(400, 300);
    entity_.addComponent<SpriteComponent>("assets/Player64x64.png", sf::Vector2f(1, 1));
    entity_.addComponent<InputComponent>();
    entity_.addComponent<UsernameComponent>(username);
    auto &sound = entity_.addComponent<SoundComponent>("assets/spaceshipIdle.mp3", -1);
    sound.sound.setPitch(0.5);
    entity_.addComponent<ScoreComponent>();
    entity_.addComponent<HealthComponent>();
}
```

## Fonctionnalités principales

### Gestion des statistiques

Suivi des points et de la santé :

```cpp
auto *score = entity.getComponent<ScoreComponent>();
if (score) {
    score->score += 10;
}
```

### Interactions utilisateur

Réagit aux entrées utilisateur pour déplacer le joueur :

```cpp
auto *input = entity.getComponent<InputComponent>();
if (input && input->moveLeft) {
    position->position.x -= 10.0f;
}
```

## Interactions

- **Avec InputSystem** : Capture les actions de l'utilisateur pour déplacer le joueur.
- **Avec MovementSystem** : Met à jour la position du joueur en fonction des entrées utilisateur.
- **Avec RenderSystem** : Affiche le joueur et ses informations à l'écran.
- **Avec SoundSystem** : Joue des sons contextuels liés aux actions du joueur.
- **Avec ScoreComponent** : Met à jour et affiche les points marqués par le joueur.

## Exemples d'Utilisation

1. **Création d'un joueur** :
   ```cpp
   PlayerEntity player(entityManager, "Player1");
   ```

2. **Mise à jour de la position** :
   ```cpp
   auto *position = playerEntity.getComponent<PositionComponent>();
   if (position) {
       position->position.x += 10.0f;
   }
   ```

3. **Suivi des points** :
   ```cpp
   auto *score = playerEntity.getComponent<ScoreComponent>();
   if (score) {
       score->score += 5;
   }
   ```

---

`PlayerEntity` est une entité essentielle pour gérer le joueur, combinant des interactions utilisateur, des statistiques et une représentation visuelle immersive.

