# ParallaxSystem

## Introduction

`ParallaxSystem` est un système conçu pour gérer les effets de défilement parallaxe dans le jeu. Il permet de créer une illusion de profondeur en déplaçant des arrière-plans ou des textures à des vitesses différentes.

## Rôle dans l'ECS

`ParallaxSystem` est utilisé pour :

- Mettre à jour les positions des entités ayant un composant `ParallaxComponent`.
- Afficher des arrière-plans dynamiques avec des mouvements fluides.
- Améliorer l'expérience visuelle en simulant une profondeur.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` ajuste la position des composants parallaxe en fonction de la vitesse définie et du temps écoulé.

#### Code extrait de `ParallaxSystem.cpp` :

```cpp
void ParallaxSystem::update(EntityManager &entityManager, float deltaTime) {
    for (auto &entity : entityManager.entities) {
        auto *parallax = entity->getComponent<ParallaxComponent>();

        if (parallax) {
            parallax->position.x += parallax->speed.x * deltaTime;
            parallax->position.y += parallax->speed.y * deltaTime;

            const auto textureSize = parallax->texture.getSize();
            if (parallax->position.x < -static_cast<float>(textureSize.x) * parallax->scale.x)
                parallax->position.x = 0;
            if (parallax->position.y < -static_cast<float>(textureSize.y) * parallax->scale.y)
                parallax->position.y = 0;

            parallax->sprite.setPosition(parallax->position);
        }
    }
}
```

### Méthode `render`

La méthode `render` dessine les textures parallaxe sur l'écran, en remplissant les espaces nécessaires pour un défilement continu.

#### Code extrait :

```cpp
void ParallaxSystem::render(sf::RenderWindow &window, EntityManager &entityManager) {
    for (auto &entity : entityManager.entities) {
        auto *parallax = entity->getComponent<ParallaxComponent>();

        if (parallax) {
            const auto textureSize = parallax->texture.getSize();
            for (float x = parallax->position.x; x < static_cast<float>(window.getSize().x); x += textureSize.x * parallax->scale.x) {
                for (float y = parallax->position.y; y < static_cast<float>(window.getSize().y); y += textureSize.y * parallax->scale.y) {
                    parallax->sprite.setPosition(x, y);
                    window.draw(parallax->sprite);
                }
            }
        }
    }
}
```

## Interactions

- **Avec ParallaxComponent** : Met à jour les positions et les vitesses définies dans le composant.
- **Avec RenderSystem** : Travaille en complément pour dessiner les arrière-plans parallaxe.

## Exemples d'Utilisation

1. **Initialisation et mise à jour** :
   ```cpp
   float deltaTime = clock.restart().asSeconds();
   parallaxSystem.update(entityManager, deltaTime);
   ```

2. **Affichage des arrière-plans** :
   ```cpp
   parallaxSystem.render(window, entityManager);
   ```

3. **Ajout d'un ParallaxComponent** :
   ```cpp
   Entity &background = entityManager.createEntity();
   background.addComponent<ParallaxComponent>("assets/background.png", sf::Vector2f(1.0f, 1.0f), sf::Vector2f(-50.0f, 0.0f));
   ```

## Fonctionnalités supplémentaires

- **Défilement continu** : Les textures sont automatiquement réinitialisées pour un effet de boucle infinie.
- **Support multi-texture** : Permet de superposer plusieurs couches de parallaxe pour un effet plus riche.

---

`ParallaxSystem` est essentiel pour améliorer l'aspect visuel du jeu en créant des arrière-plans dynamiques et immersifs.

