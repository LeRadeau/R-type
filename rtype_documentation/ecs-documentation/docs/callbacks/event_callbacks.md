# EventCallbacks

## Introduction

`EventCallbacks` contient un ensemble de fonctions utilitaires utilisées pour gérer les interactions utilisateur dans le jeu. Ces fonctions sont associées à des événements spécifiques (clics de souris, entrées clavier, etc.) et permettent d'exécuter des actions spécifiques.

## Rôle

`EventCallbacks` permet de :

- Gérer les interactions utilisateur pour des entités comme les boutons, les champs de texte ou les menus.
- Déclencher des actions en fonction des événements (lancement du jeu, fermeture de l'application, etc.).
- Simplifier la logique des systèmes en déléguant les comportements événementiels aux fonctions callback.

## Fonctions principales

### ButtonLaunchGame

Lance une partie en fermant le menu et en envoyant un message au serveur.

```cpp
void ButtonLaunchGame(MenuEntity &menu, Entity &entity, sf::RenderWindow &window, const sf::Event &event,
    std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager);
```

#### Exemple d'utilisation

```cpp
EventCallbacks::ButtonLaunchGame(menu, buttonEntity, window, event, player, networkManager);
```

### ButtonHandlePlay

Gère le clic sur le bouton "Play" en envoyant une requête de connexion au serveur et en ouvrant un lobby.

```cpp
void ButtonHandlePlay(MenuEntity &menu, Entity &entity, sf::RenderWindow &window, const sf::Event &event,
    std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager);
```

### ButtonHandleQuit

Ferme l'application et détruit toutes les entités lorsque l'utilisateur clique sur "Quit".

```cpp
void ButtonHandleQuit(EntityManager &entityManager, Entity &entity, sf::RenderWindow &window, const sf::Event &event);
```

### MenuToggle

Bascule l'affichage du menu principal en réponse à une pression sur la touche "Échap".

```cpp
void MenuToggle(const sf::Event &event, sf::RenderWindow &window, MenuEntity &menu);
```

### TextFieldListener

Ajoute des caractères dans un champ de texte en fonction des touches pressées.

```cpp
void TextFieldListener(const sf::Event &event, sf::RenderWindow &window, TextFieldEntity &textField);
```

### TextFieldCapsReleaseHandler

Désactive le verrouillage des majuscules lorsque la touche "Shift" est relâchée.

```cpp
void TextFieldCapsReleaseHandler(const sf::Event &event, sf::RenderWindow &window, TextFieldEntity &textField);
```

## Exemple de workflow

1. **Ajout d'un callback à un bouton** :
   ```cpp
   auto *eventHandler = buttonEntity.getComponent<EventHandlerComponent>();
   if (eventHandler) {
       eventHandler->setCallback(sf::Event::MouseButtonReleased, [](const sf::Event &event) {
           EventCallbacks::ButtonHandleQuit(entityManager, buttonEntity, window, event);
       });
   }
   ```

2. **Gestion d'un champ de texte** :
   ```cpp
   EventCallbacks::TextFieldListener(event, window, textField);
   ```

---

`EventCallbacks` joue un rôle essentiel pour gérer les interactions utilisateur dans le jeu, en permettant de déléguer des comportements spécifiques à des fonctions modularisées.

