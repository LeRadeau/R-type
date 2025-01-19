# SelectionComponent

## Introduction

Le composant `SelectionComponent` est utilisé pour indiquer si une entité est sélectionnée. Il est particulièrement utile pour des interactions utilisateur, comme la sélection de boutons, d'objets ou de zones dans le jeu.

## Attributs

- **isSelected** : Un booléen indiquant si l'entité est actuellement sélectionnée.

### Exemple d'attributs dans le fichier `SelectionComponent.hpp` :

```cpp
class SelectionComponent : public AComponent {
public:
    bool isSelected;

    SelectionComponent() : isSelected(false) {}
};
```

## Rôle dans l'ECS

`SelectionComponent` est utilisé pour :

- Identifier les entités qui sont activement sélectionnées par l'utilisateur.
- Permettre aux systèmes de réagir à l'état de sélection d'une entité.
- Indiquer visuellement ou logiquement les entités choisies.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &button = entityManager.createEntity();
   button.addComponent<SelectionComponent>();
   ```

2. **Vérification de la sélection** :
   ```cpp
   auto *selection = entity.getComponent<SelectionComponent>();
   if (selection && selection->isSelected) {
       std::cout << "Entité sélectionnée !" << std::endl;
   }
   ```

3. **Mise à jour de l'état de sélection** :
   ```cpp
   auto *selection = entity.getComponent<SelectionComponent>();
   if (selection) {
       selection->isSelected = true;
   }
   ```

## Interactions

- **Avec SelectionSystem** : Change l'état `isSelected` en fonction des interactions utilisateur.
- **Avec RectangleShapeComponent et SpriteComponent** : Peut être utilisé pour afficher visuellement l'état de sélection (par exemple, en changeant la couleur ou la bordure).

## Fonctionnalités principales

### Détection de la sélection

Le composant fonctionne avec des systèmes comme `SelectionSystem` pour détecter les clics ou autres interactions utilisateur qui déterminent si une entité est sélectionnée ou non.

---

Le composant `SelectionComponent` est une brique essentielle pour implémenter des interactions utilisateur intuitives, telles que la sélection d'objets ou de zones interactives.

