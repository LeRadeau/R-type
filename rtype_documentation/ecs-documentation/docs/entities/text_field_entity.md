# TextFieldEntity

## Introduction

`TextFieldEntity` représente un champ de texte interactif utilisé dans les interfaces utilisateur du jeu. Cette entité permet d'afficher des titres, de saisir des informations et de capturer des interactions utilisateur pour des actions spécifiques.

## Rôle dans l'ECS

`TextFieldEntity` est utilisé pour :

- Afficher et gérer des champs de texte interactifs.
- Permettre à l'utilisateur d'entrer des informations telles que des noms ou des adresses IP.
- Capturer et traiter les événements clavier pour interagir avec le texte.

## Attributs

Les entités `TextFieldEntity` sont créées avec les composants suivants :

- **RectangleShapeComponent** : Définit la forme et l'apparence visuelle du champ de texte.
- **TextComponent** : Affiche le texte principal et le titre associé.
- **HoverComponent** : Modifie l'apparence lorsque la souris survole le champ de texte.
- **SelectionComponent** : Indique si le champ de texte est actuellement sélectionné pour l'édition.
- **EventHandlerComponent** : Capture les événements clavier pour modifier le contenu du texte.

### Exemple de création

#### Code extrait de `TextFieldEntity.cpp` :

```cpp
TextFieldEntity::TextFieldEntity(EntityManager &entityManager, sf::RenderWindow &window, sf::Vector2f size,
    sf::Vector2f position, const sf::Font &font, const std::string &title)
    : entity_(entityManager.createEntity()), titleEntity_(entityManager.createEntity()), entityManager_(entityManager),
      window_(window)
{
    auto &eventHandlerComponent =
        entity_.addComponent<EventHandlerComponent>(sf::Event::KeyPressed, [this](const sf::Event &event) {
            EventCallbacks::TextFieldListener(event, window_, *this);
        });
    eventHandlerComponent.setCallback(sf::Event::KeyReleased, [this](const sf::Event &event) {
        EventCallbacks::TextFieldCapsReleaseHandler(event, window_, *this);
    });
    entity_.addComponent<RectangleShapeComponent>(size, position, sf::Color(128, 128, 128));
    position.y -= size.y;
    titleEntity_.addComponent<TextComponent>(title, font, position, sf::Color(208, 208, 208));
    position.y += size.y;
    entity_.addComponent<TextComponent>("", font, position, sf::Color(228, 228, 228));
    entity_.addComponent<SelectionComponent>();
    entity_.addComponent<HoverComponent>(sf::Color(128, 128, 128), sf::Color(100, 100, 100));
    conversion_ = 'a';
}
```

## Interactions

- **Avec EventHandlingSystem** : Capture les entrées utilisateur et met à jour le texte affiché.
- **Avec HoverSystem** : Change l'apparence visuelle en fonction des interactions avec la souris.
- **Avec RenderSystem** : Affiche le champ de texte et le titre associé à l'écran.

## Exemples d'Utilisation

1. **Création d'un champ de texte** :
   ```cpp
   sf::Font font;
   font.loadFromFile("assets/fonts/arial.ttf");
   TextFieldEntity textField(entityManager, window, sf::Vector2f(200, 50), sf::Vector2f(300, 400), font, "Enter Name:");
   ```

2. **Mise à jour du contenu textuel** :
   ```cpp
   auto *textComponent = textField.getComponent<TextComponent>();
   if (textComponent) {
       textComponent->data.setString("Player1");
   }
   ```

3. **Sélection et modification** :
   ```cpp
   auto *selection = textField.getComponent<SelectionComponent>();
   if (selection && selection->isSelected) {
       textField.addKey(sf::Keyboard::A);
   }
   ```

## Fonctionnalités principales

- **Gestion des événements clavier** : Capture les entrées utilisateur pour modifier dynamiquement le texte.
- **Apparence interactive** : Change visuellement en fonction de l'état de sélection ou des interactions souris.
- **Synchronisation avec d'autres entités** : Affiche des informations pertinentes pour le joueur ou les paramètres du jeu.

---

`TextFieldEntity` offre une solution puissante et flexible pour gérer les champs de texte interactifs dans les menus ou interfaces utilisateur du jeu.

