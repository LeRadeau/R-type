# SpriteComponent

## Introduction

Le composant `SpriteComponent` est utilisé pour gérer les sprites graphiques associés à une entité. Il permet d'afficher des images ou des textures pour représenter visuellement les entités dans le jeu.

## Attributs

- **sprite** : Un objet `sf::Sprite` représentant le sprite de l'entité.
- **texture** : Un objet `sf::Texture` qui stocke l'image associée au sprite.

### Exemple d'attributs dans le fichier `SpriteComponent.hpp` :

```cpp
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class SpriteComponent : public AComponent {
public:
    sf::Sprite sprite;
    sf::Texture texture;

    SpriteComponent(const std::string &texturePath, const sf::Vector2f &scale = sf::Vector2f(1.0f, 1.0f)) {
        if (!texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }
        sprite.setTexture(texture);
        sprite.setScale(scale);
    }
};
```

## Rôle dans l'ECS

`SpriteComponent` est utilisé pour :

- Afficher des entités avec des textures personnalisées.
- Gérer les propriétés visuelles, comme l'échelle et la position des sprites.
- Faciliter l'affichage dynamique des entités dans le jeu.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &player = entityManager.createEntity();
   player.addComponent<SpriteComponent>("assets/player.png");
   ```

2. **Modification de la texture** :
   ```cpp
   auto *sprite = entity.getComponent<SpriteComponent>();
   if (sprite) {
       sprite->texture.loadFromFile("assets/new_texture.png");
       sprite->sprite.setTexture(sprite->texture);
   }
   ```

3. **Positionnement** :
   ```cpp
   auto *sprite = entity.getComponent<SpriteComponent>();
   auto *position = entity.getComponent<PositionComponent>();
   if (sprite && position) {
       sprite->sprite.setPosition(position->position);
   }
   ```

## Interactions

- **Avec PositionComponent** : Positionne le sprite à l'endroit approprié.
- **Avec RenderSystem** : Dessine les sprites des entités à l'écran.
- **Avec HoverComponent** : Change l'apparence du sprite en réponse à des interactions utilisateur.

## Fonctionnalités supplémentaires

- **Support de l'échelle** : Permet de redimensionner les sprites pour s'adapter à différents contextes visuels.
- **Flexibilité graphique** : Peut être utilisé avec des textures animées ou statiques.

---

`SpriteComponent` est un composant essentiel pour gérer les visuels des entités dans le jeu, offrant une flexibilité pour afficher des images et des textures.

