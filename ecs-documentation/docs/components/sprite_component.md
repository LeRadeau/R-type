# SpriteComponent

## Introduction

Le composant `SpriteComponent` est conçu pour gérer les sprites graphiques associés à une entité. Il permet d'afficher des textures complexes, telles que des personnages ou des objets, en utilisant des images.

## Attributs

- **sprite** : Un objet `sf::Sprite` qui contient la texture et les informations associées pour le rendu.
- **texture** : Un objet `sf::Texture` qui stocke l'image utilisée par le sprite.

### Exemple d'attributs dans le fichier `SpriteComponent.hpp` :

```cpp
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class SpriteComponent : public AComponent {
public:
    sf::Sprite sprite;
    sf::Texture texture;

    SpriteComponent(const std::string &texturePath) {
        if (!texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }
        sprite.setTexture(texture);
    }
};
```

## Rôle dans l'ECS

`SpriteComponent` est utilisé pour :

- Afficher des textures personnalisées sur les entités.
- Gérer les animations ou les changements d'apparence en modifiant les propriétés du sprite.
- Compléter `RenderComponent` pour des affichages graphiques plus détaillés.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &entity = entityManager.createEntity();
   entity.addComponent<SpriteComponent>("assets/player.png");
   ```

2. **Modification de la texture** :
   ```cpp
   auto *sprite = entity->getComponent<SpriteComponent>();
   if (sprite) {
       if (!sprite->texture.loadFromFile("assets/enemy.png")) {
           throw std::runtime_error("Failed to load texture");
       }
       sprite->sprite.setTexture(sprite->texture);
   }
   ```

3. **Rendu dans un système** :
   Utilisé par `RenderSystem` pour dessiner le sprite :
   ```cpp
   void RenderSystem::update(EntityManager &entityManager) {
       for (auto &entity : entityManager.entities) {
           auto *sprite = entity->getComponent<SpriteComponent>();
           auto *position = entity->getComponent<PositionComponent>();
           if (sprite && position) {
               sprite->sprite.setPosition(position->position);
               window.draw(sprite->sprite);
           }
       }
   }
   ```

## Interactions

- **Avec PositionComponent** : Positionne le sprite à l'endroit approprié sur l'écran.
- **Avec AnimationSystem** : Permet de gérer les animations en modifiant les textures ou les propriétés du sprite.

---

Le composant `SpriteComponent` est idéal pour des entités nécessitant des visuels complexes, offrant une flexibilité pour le rendu d'images dans le jeu.

