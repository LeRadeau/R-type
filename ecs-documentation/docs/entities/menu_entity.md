# MenuEntity

## Introduction

`MenuEntity` est une entité dédiée à la gestion des interfaces utilisateur, telles que le menu principal et les lobbys. Elle combine plusieurs composants pour créer une expérience interactive fluide.

## Rôle dans l'ECS

`MenuEntity` est utilisé pour :

- Gérer les menus interactifs et les lobbys.
- Fournir des boutons et champs de texte pour des interactions utilisateur dynamiques.
- Synchroniser l'état du jeu entre le client et le serveur via des callbacks réseau.

## Attributs

- **Boutons** : Une liste de `ButtonEntity` pour les actions interactives (ex. : "Jouer", "Quitter").
- **Champs de texte** : Des instances de `TextFieldEntity` pour saisir des informations (ex. : nom d'utilisateur, adresse IP).
- **Callbacks réseau** : Gestion dynamique des messages réseau via `NetworkCallbackComponent`.

### Exemple de création

#### Code extrait de `MenuEntity.cpp`

```cpp
MenuEntity::MenuEntity(EntityManager &entityManager, sf::RenderWindow &window, const sf::Font &font,
    std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager)
    : entity_(entityManager.createEntity()), entityManager_(entityManager), networkManager_(networkManager),
      window_(window), font_(font), player_(player), entityText_(entityManager.createEntity())
{
    open();
    entity_.addComponent<EventHandlerComponent>(sf::Event::KeyReleased, [this, &window](const sf::Event &event) {
        EventCallbacks::MenuToggle(event, window, *this);
    });
}
```

## Fonctionnalités principales

### Gestion des lobbys

Permet d'ouvrir ou fermer un lobby en ajoutant des boutons et des textes dynamiques :

```cpp
void MenuEntity::openLobby() {
    sf::Vector2f size(275, 50);
    sf::Vector2f position(window_.getSize().x / 2.0f - 300, 500);
    entityText_.addComponent<TextComponent>("?/4 players", font_, position, sf::Color::White);
    auto &comp = entityText_.addComponent<NetworkCallbackComponent>();
    comp.setCallback(MessageType::WAIT, [this](const char *&packet) {
        NetworkCallbacks::onWaitUpdateClientNbr(packet, entityText_);
    });
}
```

## Interactions

- **Avec ButtonEntity** : Gère les boutons interactifs dans les menus.
- **Avec TextFieldEntity** : Capture les entrées utilisateur pour des champs de texte.
- **Avec NetworkManager** : Synchronise les états des lobbys avec le serveur.
- **Avec RenderSystem** : Affiche les menus et leurs éléments visuels.

## Exemples d'Utilisation

1. **Création d'un menu** :
   ```cpp
   MenuEntity menu(entityManager, window, font, playerEntity, networkManager);
   ```

2. **Ajout de boutons** :
   ```cpp
   menu.open();
   ```

3. **Synchronisation réseau** :
   ```cpp
   menu.openLobby();
   ```

---

`MenuEntity` est une solution complète pour gérer les interfaces utilisateur dans le jeu, offrant des interactions dynamiques et une synchronisation fluide avec le serveur.

