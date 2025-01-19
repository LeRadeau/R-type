# SelectionSystem

## Introduction

`SelectionSystem` gère la sélection des entités dans le jeu en détectant les clics de souris. Il modifie l'état des entités ayant un composant `SelectionComponent` pour indiquer si elles sont sélectionnées.

## Rôle dans l'ECS

`SelectionSystem` est utilisé pour :

- Détecter les clics de souris sur des entités interactives.
- Mettre à jour l'état de sélection des entités avec un composant `SelectionComponent`.
- Permettre des interactions utilisateur intuitives dans les menus ou sur les objets du jeu.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` vérifie si une entité a été sélectionnée en fonction des événements de clic de souris.

#### Code extrait de `SelectionSystem.cpp` :

```cpp
void SelectionSystem::update(EntityManager &entityManager, const sf::Event::MouseButtonEvent &mouseEvent)
{
    if (mouseEvent.button != sf::Mouse::Left)
        return;
    for (auto &i : entityManager.entities) {
        auto *spriteComponent = i->getComponent<SpriteComponent>();
        auto *selectionComponent = i->getComponent<SelectionComponent>();
        auto *rectangleShapeComponent = i->getComponent<RectangleShapeComponent>();

        if (spriteComponent && selectionComponent) {
            selectionComponent->isSelected =
                spriteComponent->sprite.getGlobalBounds().contains(mouseEvent.x, mouseEvent.y);
        }
        if (rectangleShapeComponent && selectionComponent) {
            selectionComponent->isSelected =
                rectangleShapeComponent->shape.getGlobalBounds().contains(mouseEvent.x, mouseEvent.y);
        }
    }
}
```

## Interactions

- **Avec SelectionComponent** : Modifie l'état `isSelected` en fonction des interactions utilisateur.
- **Avec SpriteComponent** : Vérifie si le clic de souris correspond à un sprite.
- **Avec RectangleShapeComponent** : Vérifie si le clic de souris correspond à une forme rectangulaire.

## Exemples d'Utilisation

1. **Initialisation et mise à jour** :
   ```cpp
   sf::Event event;
   if (event.type == sf::Event::MouseButtonPressed) {
       selectionSystem.update(entityManager, event.mouseButton);
   }
   ```

2. **Ajout d'un composant de sélection** :
   ```cpp
   Entity &button = entityManager.createEntity();
   button.addComponent<SelectionComponent>();
   ```

3. **Vérification de la sélection** :
   ```cpp
   auto *selection = entity.getComponent<SelectionComponent>();
   if (selection && selection->isSelected) {
       std::cout << "Entité sélectionnée !" << std::endl;
   }
   ```

## Fonctionnalités supplémentaires

- **Support multi-formes** : Prend en charge les entités ayant des sprites ou des formes rectangulaires.
- **Interactions utilisateur dynamiques** : Facilite la création de menus et de systèmes interactifs.

---

`SelectionSystem` est un système clé pour gérer les interactions utilisateur en permettant la sélection d'entités dans le jeu.

