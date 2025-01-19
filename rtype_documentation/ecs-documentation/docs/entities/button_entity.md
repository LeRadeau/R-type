# ButtonEntity

## Introduction

`ButtonEntity` est une entité utilisée pour représenter des boutons interactifs dans l'interface utilisateur du jeu. Elle permet de gérer les interactions utilisateur via des clics de souris ou d'autres événements.

## Rôle dans l'ECS

`ButtonEntity` est utilisé pour :

- Afficher des boutons visuels dans les menus ou interfaces utilisateur.
- Réagir aux clics et exécuter des actions associées.
- Simplifier la création et la gestion des boutons interactifs dans le jeu.

## Attributs

Les entités `ButtonEntity` incluent les composants suivants :

- **HoverComponent** : Change l'apparence du bouton lorsque la souris le survole.
- **RectangleShapeComponent** : Définit la forme et l'apparence visuelle du bouton.
- **TextComponent** : Affiche le texte associé au bouton.
- **EventHandlerComponent** : Gère les événements utilisateur pour le bouton.

### Exemple de création

#### Code extrait de `ButtonEntity.cpp`

```cpp
ButtonEntity::ButtonEntity(EntityManager &entityManager, sf::Vector2f size, sf::Vector2f position,
    const std::string &text, const sf::Font &font)
    : entityManager_(entityManager), entity_(entityManager.createEntity())
{
    entity_.addComponent<HoverComponent>(sf::Color(128, 128, 128), sf::Color(128, 128, 128, 128));
    entity_.addComponent<RectangleShapeComponent>(size, position, sf::Color(128, 128, 128));
    entity_.addComponent<TextComponent>(text, font, position, sf::Color::White);
}

Entity &ButtonEntity::createButtonEntity(EntityManager &entityManager, sf::Vector2f size, sf::Vector2f position,
    const std::string &text, const sf::Font &font)
{
    Entity &entity = entityManager.createEntity();
    entity.addComponent<HoverComponent>(sf::Color(128, 128, 128), sf::Color(128, 128, 128, 128));
    entity.addComponent<RectangleShapeComponent>(size, position, sf::Color(128, 128, 128));
    entity.addComponent<TextComponent>(text, font, position, sf::Color::White);
    return entity;
}
```

## Fonctionnalités principales

### Gestion des interactions utilisateur

Permet d'associer des callbacks à des événements spécifiques :

```cpp
void ButtonEntity::setCallback(sf::Event::EventType eventType, EventHandlerComponent::callbackFunction callback)
{
    auto *eventHandlerComponent = entity_.getComponent<EventHandlerComponent>();
    if (!eventHandlerComponent)
        eventHandlerComponent = &entity_.addComponent<EventHandlerComponent>(eventType, callback);
    eventHandlerComponent->setCallback(eventType, callback);
}
```

## Interactions

- **Avec HoverSystem** : Change la couleur ou l'apparence du bouton lorsqu'il est survolé par la souris.
- **Avec RenderSystem** : Affiche le bouton à l'écran avec ses propriétés visuelles.
- **Avec EventHandlingSystem** : Exécute les actions associées lorsqu'un bouton est cliqué.

## Exemples d'Utilisation

1. **Création d'un bouton** :
   ```cpp
   ButtonEntity button(entityManager, sf::Vector2f(150, 50), sf::Vector2f(300, 400), "Play", font);
   ```

2. **Ajout d'un callback** :
   ```cpp
   button.setCallback(sf::Event::MouseButtonReleased, [](const sf::Event &event) {
       std::cout << "Bouton cliqué !" << std::endl;
   });
   ```

---

`ButtonEntity` est un outil clé pour créer des interfaces interactives, en fournissant des fonctionnalités dynamiques et intuitives pour les boutons dans le jeu.

