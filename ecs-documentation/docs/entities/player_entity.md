# PlayerEntity

## Introduction

`PlayerEntity` représente le joueur dans le jeu. Cette entité contient des composants qui permettent de gérer l'apparence, les entrées utilisateur, le son, et les interactions réseau pour le joueur.

## Rôle dans l'ECS

`PlayerEntity` est utilisé pour :

- Représenter le joueur avec une position et un sprite visuel.
- Gérer les interactions du joueur à travers les entrées clavier.
- Jouer des sons associés au joueur.
- Synchroniser les actions du joueur avec le serveur.

## Attributs

Les entités `PlayerEntity` sont créées avec les composants suivants :

- **PositionComponent** : Définit la position initiale du joueur.
- **SpriteComponent** : Représente graphiquement le joueur avec un sprite.
- **InputComponent** : Capture les entrées utilisateur pour contrôler le joueur.
- **UsernameComponent** : Identifie le joueur par un nom unique.
- **SoundComponent** : Ajoute des effets sonores liés au joueur.

### Exemple de création

#### Code extrait de `PlayerEntity.cpp` :

```cpp
PlayerEntity::PlayerEntity(EntityManager &entityManager, const std::string &username, NetworkManager &networkManager)
    : entityManager_(entityManager), entity_(entityManager.createEntity())
{
    entity_.addComponent<PositionComponent>(400, 300);
    entity_.addComponent<SpriteComponent>("assets/Player64x64.png", sf::Vector2f(1, 1));
    entity_.addComponent<InputComponent>();
    entity_.addComponent<UsernameComponent>(username);
    auto &sound = entity_.addComponent<SoundComponent>("assets/spaceshipIdle.mp3", -1);
    sound.sound.setPitch(0.5);
    networkManager.send(MessageType::CONNECT, username);
}
```

## Interactions

- **Avec InputSystem** : Utilise les entrées utilisateur pour contrôler les mouvements et les actions du joueur.
- **Avec MovementSystem** : Met à jour la position du joueur en fonction des entrées.
- **Avec RenderSystem** : Affiche le sprite et les informations associées au joueur.
- **Avec SoundSystem** : Joue les sons liés aux actions du joueur.

## Exemples d'Utilisation

1. **Création d'un joueur** :
   ```cpp
   sf::Font font;
   font.loadFromFile("assets/fonts/arial.ttf");
   PlayerEntity player(entityManager, "Player1", networkManager);
   ```

2. **Mise à jour de la position** :
   ```cpp
   auto *position = playerEntity.getComponent<PositionComponent>();
   if (position) {
       position->position.x += 10.0f;
   }
   ```

3. **Réagir aux entrées utilisateur** :
   ```cpp
   auto *input = playerEntity.getComponent<InputComponent>();
   if (input && input->moveLeft) {
       std::cout << "Déplacement vers la gauche !" << std::endl;
   }
   ```

## Fonctionnalités principales

- **Gestion des entrées** : Capture et traite les actions du joueur.
- **Synchronisation réseau** : Transmet les actions du joueur au serveur.
- **Effets sonores** : Joue des sons contextuels liés au joueur.

---

`PlayerEntity` est une entité clé qui permet de représenter et de gérer le joueur dans le jeu, en intégrant des interactions visuelles, sonores et réseau.

