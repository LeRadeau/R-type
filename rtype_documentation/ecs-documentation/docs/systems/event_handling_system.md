# EventHandlingSystem

## Introduction

`EventHandlingSystem` est un système conçu pour gérer les interactions utilisateur en écoutant et en traitant les événements SFML. Il exécute les callbacks associés aux entités disposant de composants `EventHandlerComponent`.

## Rôle dans l'ECS

`EventHandlingSystem` est utilisé pour :

- Écouter les événements utilisateur, tels que les clics de souris ou les pressions de touches.
- Exécuter les fonctions de callback définies dans les composants `EventHandlerComponent`.
- Assurer une gestion fluide des interactions entre les entités et l'utilisateur.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` parcourt toutes les entités et exécute les callbacks associés aux événements déclenchés.

#### Code extrait de `EventHandlingSystem.cpp` :

```cpp
void EventHandlingSystem::update(EntityManager &entityManager, const sf::Event &event)
{
    for (size_t i = 0; i < entityManager.entities.size(); i++) {
        auto &entity = entityManager.entities[i];
        auto *eventHandlerComponent = entity->getComponent<EventHandlerComponent>();

        if (eventHandlerComponent) {
            if (eventHandlerComponent->callbacks.find(event.type) != eventHandlerComponent->callbacks.end())
                try {
                    eventHandlerComponent->callbacks.at(event.type)(event);
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
        }
    }
    entityManager.destroyMarkedEntities();
}
```

## Interactions

- **Avec EventHandlerComponent** : Exécute les callbacks définis dans ce composant en fonction des événements reçus.
- **Avec EntityManager** : Parcourt les entités et applique les changements nécessaires.

## Exemples d'Utilisation

1. **Initialisation et utilisation** :
   ```cpp
   sf::Event event;
   while (window.pollEvent(event)) {
       eventHandlingSystem.update(entityManager, event);
   }
   ```

2. **Ajout d'un callback à une entité** :
   ```cpp
   Entity &button = entityManager.createEntity();
   button.addComponent<EventHandlerComponent>(sf::Event::MouseButtonPressed, [](const sf::Event &event) {
       std::cout << "Bouton cliqué !" << std::endl;
   });
   ```

## Fonctionnalités supplémentaires

- **Gestion des exceptions** : Capture les erreurs lors de l'exécution des callbacks pour éviter les interruptions.
- **Destruction des entités** : Supprime les entités marquées après la gestion des événements.

---

`EventHandlingSystem` est essentiel pour implémenter des interactions dynamiques et réactives dans le jeu, en connectant les événements utilisateur aux actions spécifiques des entités.

