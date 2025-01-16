# ParallaxComponent

## Introduction

Le composant `ParallaxComponent` est utilisé pour créer des effets de défilement parallaxe dans le jeu. Il permet d'ajouter de la profondeur visuelle en déplaçant des textures ou des images à différentes vitesses.

## Attributs

- **texture** : La texture utilisée pour l'effet de parallaxe.
- **sprite** : Un sprite associé à la texture.
- **position** : La position actuelle du parallaxe.
- **scale** : Le facteur d'échelle appliqué à la texture.
- **speed** : La vitesse de défilement sur les axes X et Y.

### Exemple d'attributs dans le fichier `ParallaxComponent.hpp` :

```cpp
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

class ParallaxComponent : public AComponent {
public:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f scale;
    sf::Vector2f speed;

    ParallaxComponent(const std::string &texturePath, const sf::Vector2f &scale, const sf::Vector2f &speed)
        : scale(scale), speed(speed) {
        if (!texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }
        sprite.setTexture(texture);
        sprite.setScale(scale);
    }
};
```

## Rôle dans l'ECS

`ParallaxComponent` est utilisé pour :

- Ajouter des arrière-plans ou des éléments décoratifs avec des effets de défilement parallaxe.
- Créer une profondeur visuelle dans le jeu.
- Synchroniser le mouvement du parallaxe avec les actions ou événements du jeu.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &background = entityManager.createEntity();
   background.addComponent<ParallaxComponent>("assets/background.png", sf::Vector2f(1.0f, 1.0f), sf::Vector2f(-50.0f, 0.0f));
   ```

2. **Modification des propriétés** :
   ```cpp
   auto *parallax = entity.getComponent<ParallaxComponent>();
   if (parallax) {
       parallax->speed = sf::Vector2f(-30.0f, 0.0f);
   }
   ```

## Interactions

- **Avec ParallaxSystem** : Le système met à jour les positions des composants parallaxe en fonction de la vitesse et du temps écoulé.
- **Avec RenderSystem** : Affiche les sprites parallaxe sur l'écran.

## Fonctionnalités principales

### Effet de défilement

Le composant fonctionne en tandem avec `ParallaxSystem` pour déplacer la texture de manière fluide, en créant un effet de continuité visuelle même lorsque la texture sort de l'écran.

---

Le composant `ParallaxComponent` est idéal pour enrichir l'expérience visuelle du joueur, en apportant des éléments décoratifs dynamiques et immersifs.

