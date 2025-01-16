# MenuEntity

## Introduction

`MenuEntity` représente les interfaces utilisateur du jeu, telles que les menus principaux ou les écrans de paramètres. Elle fournit des boutons interactifs et des champs de texte permettant aux joueurs d'interagir avec le jeu.

## Rôle dans l'ECS

`MenuEntity` est utilisé pour :

- Construire et gérer les menus interactifs.
- Fournir des interfaces conviviales avec des boutons et des champs de texte.
- Connecter les interactions utilisateur avec les fonctionnalités du jeu.

## Attributs

Les entités `MenuEntity` sont composées des éléments suivants :

- **Boutons** : Des instances de `ButtonEntity` pour gérer les actions interactives.
- **Champs de texte** : Des instances de `TextFieldEntity` pour saisir ou afficher des informations.
- **EventHandlerComponent** : Pour gérer les événements utilisateur (par exemple, appuyer sur des touches ou cliquer sur un bouton).

### Exemple de création

#### Code extrait de `MenuEntity.cpp` :

```cpp
MenuEntity::MenuEntity(EntityManager &entityManager, sf::RenderWindow &window, const sf::Font &font,
    std::unique_ptr<PlayerEntity> &player, NetworkManager &networkManager)
    : entity_(entityManager.createEntity()), entityManager_(entityManager), networkManager_(networkManager),
      window_(window), font_(font), player_(player)
{
    open();
    entity_.addComponent<EventHandlerComponent>(sf::Event::KeyReleased, [this, &window](const sf::Event &event) {
        EventCallbacks::MenuToggle(event, window, *this);
    });
}
```

## Interactions

- **Avec ButtonEntity** : Les boutons sont des éléments clés des menus et déclenchent des actions spécifiques (comme "Jouer" ou "Quitter").
- **Avec TextFieldEntity** : Permet d'entrer des informations utilisateur, telles que l'adresse IP ou le nom d'utilisateur.
- **Avec RenderSystem** : Affiche les menus et leurs éléments visuels à l'écran.

## Fonctionnalités principales

### Gestion des boutons et des champs de texte

Les boutons et champs de texte sont ajoutés dynamiquement au menu. Par exemple :

```cpp
void MenuEntity::open()
{
    sf::Vector2f size(150, 50);
    sf::Vector2f position(window_.getSize().x / 2.0f - 300, 500);
    buttons_.push_back(std::make_unique<ButtonEntity>(entityManager_, size, position, "Play", font_));
    buttons_[0]->setCallback(sf::Event::MouseButtonReleased, [this](const sf::Event &event) {
        EventCallbacks::ButtonHandlePlay(
            *this, buttons_[0]->getEntity(), window_, event, entityManager_, player_, networkManager_);
    });
}
```

## Exemples d'Utilisation

1. **Création d'un menu** :
   ```cpp
   sf::Font font;
   font.loadFromFile("assets/fonts/arial.ttf");
   MenuEntity menu(entityManager, window, font, playerEntity, networkManager);
   ```

2. **Ajout d'un champ de texte** :
   ```cpp
   position.x = 400;
   position.y = 300;
   auto textField = std::make_unique<TextFieldEntity>(entityManager, window, sf::Vector2f(200, 50), position, font, "Enter Name:");
   ```

## Gestion dynamique

Le menu peut être ouvert et fermé dynamiquement, en créant ou supprimant ses éléments selon les besoins.

---

`MenuEntity` fournit une infrastructure robuste pour créer des menus interactifs dans le jeu, facilitant les interactions utilisateur avec des éléments intuitifs et personnalisables.

