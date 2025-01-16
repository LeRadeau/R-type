# ButtonEntity

## Introduction

`ButtonEntity` représente une entité interactive utilisée pour créer des boutons dans l'interface du jeu. Elle permet d'associer des événements et des actions spécifiques, comme des clics de souris.

## Rôle dans l'ECS

`ButtonEntity` est utilisé pour :

- Afficher des boutons interactifs.
- Réagir aux événements de la souris, comme les clics ou le survol.
- Fournir une base pour créer des interfaces utilisateur dynamiques.

## Attributs

Les entités boutons sont créées avec les composants suivants :

- **HoverComponent** : Change l'apparence du bouton lorsqu'il est survolé par la souris.
- **RectangleShapeComponent** : Définit la forme et l'apparence visuelle du bouton.
- **TextComponent** : Affiche le texte du bouton.
- **EventHandlerComponent** : Associe des événements (clics de souris, survol, etc.) à des callbacks spécifiques.

### Exemple de création

#### Code extrait de `ButtonEntity.cpp` :

```cpp
ButtonEntity::ButtonEntity(EntityManager &entityManager, sf::Vector2f size, sf::Vector2f position,
    const std::string &text, const sf::Font &font)
    : entityManager_(entityManager), entity_(entityManager.createEntity())
{
    entity_.addComponent<HoverComponent>(sf::Color(128, 128, 128), sf::Color(128, 128, 128, 128));
    entity_.addComponent<RectangleShapeComponent>(size, position, sf::Color(128, 128, 128));
    entity_.addComponent<TextComponent>(text, font, position, sf::Color::White);
}

void ButtonEntity::setCallback(sf::Event::EventType eventType, EventHandlerComponent::callbackFunction callback)
{
    auto *eventHandlerComponent = entity_.getComponent<EventHandlerComponent>();
    if (!eventHandlerComponent)
        eventHandlerComponent = &entity_.addComponent<EventHandlerComponent>(eventType, callback);
    eventHandlerComponent->setCallback(eventType, callback);
}
```

## Interactions

- **Avec HoverSystem** : Change la couleur du bouton lorsqu'il est survolé.
- **Avec EventHandlingSystem** : Exécute les callbacks associés lorsqu'un événement est détecté.
- **Avec RenderSystem** : Affiche le bouton et son texte à l'écran.

## Exemples d'Utilisation

1. **Création d'un bouton** :
   ```cpp
   sf::Font font;
   font.loadFromFile("assets/fonts/arial.ttf");
   ButtonEntity button(entityManager, sf::Vector2f(150, 50), sf::Vector2f(300, 400), "Play", font);
   ```

2. **Ajout d'un callback** :
   ```cpp
   button.setCallback(sf::Event::MouseButtonReleased, [](const sf::Event &event) {
       std::cout << "Bouton cliqué !" << std::endl;
   });
   ```

---

`ButtonEntity` est un outil essentiel pour créer des interfaces interactives dans le jeu, permettant des interactions fluides et dynamiques avec les utilisateurs.

