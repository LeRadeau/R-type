# ScoreComponent

## Introduction

Le composant `ScoreComponent` est utilisé pour suivre et gérer les points marqués par une entité, généralement associée à un joueur ou à un allié. Il permet de gérer des scores dans le jeu, comme le nombre d'ennemis vaincus ou d'objectifs atteints.

## Attributs

- **score** : Un entier représentant le score actuel de l'entité.

### Exemple d'attributs dans le fichier `ScoreComponent.hpp` :

```cpp
class ScoreComponent : public AComponent {
public:
    int score;

    ScoreComponent() : score(0) {}
};
```

## Rôle dans l'ECS

`ScoreComponent` est utilisé pour :

- Enregistrer le score des entités associées.
- Permettre à des systèmes ou entités d'interagir avec le score (par exemple, l'incrémenter ou l'afficher).
- Faciliter la synchronisation des scores dans des environnements multijoueurs.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<ScoreComponent>().score = 0;
   ```

2. **Mise à jour du score** :
   ```cpp
   auto *scoreComponent = player.getComponent<ScoreComponent>();
   if (scoreComponent) {
       scoreComponent->score += 10;
   }
   ```

3. **Affichage du score** :
   Utilisé dans un système comme `RenderSystem` :
   ```cpp
   if (scoreComponent) {
       sf::Text scoreText;
       scoreText.setString("Score: " + std::to_string(scoreComponent->score));
       window.draw(scoreText);
   }
   ```

## Interactions

- **Avec RenderSystem** : Utilisé pour afficher le score des entités à l'écran.
- **Avec MessageSystem** : Permet de synchroniser les scores entre le client et le serveur dans des environnements multijoueurs.
- **Avec HealthComponent** : Peut être mis à jour lorsqu'une entité est détruite ou vaincue.

## Fonctionnalités supplémentaires

- **Facilité d'intégration** : Peut être ajouté à n'importe quelle entité pour suivre son score.
- **Synchronisation réseau** : Permet de partager les scores entre plusieurs instances du jeu.

---

`ScoreComponent` est un outil simple mais efficace pour gérer les scores dans le jeu, ajoutant une couche de compétition ou de progression.

