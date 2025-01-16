# EventHandlerComponent

## Introduction

Le composant `EventHandlerComponent` permet de gérer les événements associés à une entité. Il permet de définir des callbacks qui sont exécutés en réponse à des événements spécifiques, tels que les clics de souris ou les pressions de touches.

## Attributs

- **callbacks** : Une table associant un type d'événement (`sf::Event::EventType`) à une fonction de callback.

### Exemple d'attributs dans le fichier `EventHandlerComponent.hpp` :

```cpp
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <functional>

class EventHandlerComponent : public AComponent {
public:
    using callbackFunction = std::function<void(const sf::Event &)>;

    std::unordered_map<sf::Event::EventType, callbackFunction> callbacks;

    EventHandlerComponent(sf::Event::EventType eventType, callbackFunction callback) {
        callbacks[eventType] = callback;
    }

    void setCallback(sf::Event::EventType eventType, callbackFunction callback) {
        callbacks[eventType] = callback;
    }
};
```

## Rôle dans l'ECS

`EventHandlerComponent` est utilisé pour :

- Associer des événements à des actions spécifiques sur une entité.
- Fournir une gestion flexible des interactions utilisateur.
- Permettre une réaction contextuelle des entités aux événements du jeu.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &button = entityManager.createEntity();
   button.addComponent<EventHandlerComponent>(sf::Event::MouseButtonPressed, [](const sf::Event &event) {
       std::cout << "Bouton cliqué !" << std::endl;
   });
   ```

2. **Modification des callbacks** :
   ```cpp
   auto *eventHandler = entity.getComponent<EventHandlerComponent>();
   if (eventHandler) {
       eventHandler->setCallback(sf::Event::KeyPressed, [](const sf::Event &event) {
           std::cout << "Touche pressée !" << std::endl;
       });
   }
   ```

## Interactions

- **Avec EventHandlingSystem** : Le système exécute les callbacks définis dans ce composant pour chaque événement détecté.
- **Avec HoverComponent** : Peut être utilisé pour déclencher des événements en réponse au survol de la souris.

---

Le composant `EventHandlerComponent` est essentiel pour la gestion des interactions utilisateur dans le jeu, offrant une flexibilité pour associer des événements à des comportements spécifiques.

