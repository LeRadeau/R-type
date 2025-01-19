# InputComponent

## Introduction

Le composant `InputComponent` gère les entrées utilisateur associées à une entité spécifique. Il permet de capturer et de traiter les actions comme le déplacement, les tirs ou d'autres commandes.

## Attributs

- **moveLeft** : Booléen indiquant si l'utilisateur presse la touche de déplacement à gauche.
- **moveRight** : Booléen indiquant si l'utilisateur presse la touche de déplacement à droite.
- **moveUp** : Booléen indiquant si l'utilisateur presse la touche de déplacement vers le haut.
- **moveDown** : Booléen indiquant si l'utilisateur presse la touche de déplacement vers le bas.
- **spaceBar** : Booléen indiquant si l'utilisateur presse la barre d'espace.

### Exemple d'attributs dans le fichier `InputComponent.hpp` :

```cpp
class InputComponent : public AComponent {
public:
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    bool spaceBar = false;
};
```

## Rôle dans l'ECS

`InputComponent` est utilisé pour :

- Capturer les entrées utilisateur et les associer à une entité donnée.
- Permettre aux systèmes (comme `InputSystem`) de traduire ces entrées en actions concrètes dans le jeu.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<InputComponent>();
   ```

2. **Traitement des entrées** :
   Utilisé dans `InputSystem` pour mettre à jour les états des composants :
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

## Interactions

- **Avec MovementSystem** : Traduit les entrées utilisateur en déplacements d'entités.
- **Avec NetworkManager** : Synchronise les commandes utilisateur avec le serveur pour un jeu en réseau.

---

Le composant `InputComponent` est crucial pour connecter les actions des joueurs aux entités dans le jeu, en traduisant les entrées utilisateur en modifications d'état concrètes.

