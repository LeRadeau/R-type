# InputSystem

## Introduction

Le système `InputSystem` est responsable de la gestion des entrées utilisateur et de leur traduction en actions sur les entités correspondantes. Il détecte les touches pressées et met à jour les composants d'entrée des entités concernées.

## Rôle dans l'ECS

`InputSystem` a pour rôle de :

- Capturer les entrées utilisateur en temps réel.
- Mettre à jour les états des composants `InputComponent` pour refléter les actions actuelles.
- Fournir une interface permettant aux autres systèmes de réagir aux actions utilisateur.

## Fonctionnalités principales

### Méthode `update`

Cette méthode parcourt toutes les entités qui possèdent un `InputComponent` et met à jour leurs attributs en fonction des touches pressées.

#### Exemple de code

```cpp
void InputSystem::update(EntityManager &entityManager) {
    for (auto &entity : entityManager.entities) {
        auto *input = entity->getComponent<InputComponent>();
        if (input) {
            input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
            input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        }
    }
}
```

### Gestion des interactions

Les entrées capturées par ce système sont ensuite utilisées par d'autres systèmes tels que `MovementSystem` ou `RenderSystem` pour influencer les actions du jeu.

## Exemples d'Utilisation

1. **Initialisation et appel** :
   ```cpp
   InputSystem inputSystem;
   inputSystem.update(entityManager);
   ```

2. **Réagir aux entrées utilisateur** :
   Les composants mis à jour par ce système sont ensuite utilisés pour déplacer les entités ou déclencher des actions spécifiques.

## Interactions

- **Avec InputComponent** : Met à jour les attributs de ce composant en fonction des entrées utilisateur.
- **Avec MovementSystem** : Fournit les données nécessaires pour déplacer les entités en fonction des commandes utilisateur.
- **Avec NetworkManager** : Synchronise les commandes utilisateur avec le serveur dans un environnement multijoueur.

---

Le système `InputSystem` est un élément clé pour connecter les actions des joueurs aux entités du jeu, garantissant une interaction fluide et réactive.

