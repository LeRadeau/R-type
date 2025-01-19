# TextComponent

## Introduction

Le composant `TextComponent` est utilisé pour afficher du texte associé à une entité. Il permet de configurer et de styliser le texte, ainsi que de le positionner dans le monde du jeu.

## Attributs

- **data** : Un objet `sf::Text` représentant le texte affiché.
- **font** : Un objet `sf::Font` utilisé pour styler le texte.

### Exemple d'attributs dans le fichier `TextComponent.hpp` :

```cpp
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextComponent : public AComponent {
public:
    sf::Text data;
    sf::Font font;

    TextComponent(const std::string &text, const sf::Font &font, const sf::Vector2f &position, const sf::Color &color)
        : font(font) {
        data.setFont(this->font);
        data.setString(text);
        data.setPosition(position);
        data.setFillColor(color);
    }
};
```

## Rôle dans l'ECS

`TextComponent` est utilisé pour :

- Afficher du texte contextuel, tel que des noms d'utilisateur ou des informations de jeu.
- Gérer les propriétés du texte, comme la taille de la police, la couleur et le style.
- Synchroniser les textes avec les états des entités.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &playerLabel = entityManager.createEntity();
   playerLabel.addComponent<TextComponent>("Player1", font, sf::Vector2f(100.0f, 200.0f), sf::Color::White);
   ```

2. **Modification du texte** :
   ```cpp
   auto *text = entity.getComponent<TextComponent>();
   if (text) {
       text->data.setString("Nouveau Texte");
   }
   ```

3. **Changement de couleur** :
   ```cpp
   auto *text = entity.getComponent<TextComponent>();
   if (text) {
       text->data.setFillColor(sf::Color::Red);
   }
   ```

## Interactions

- **Avec RenderSystem** : Le système dessine les textes des entités à l'écran.
- **Avec EventHandlerComponent** : Permet de mettre à jour dynamiquement le texte en fonction des événements du jeu.

## Fonctionnalités principales

### Affichage de texte dynamique

Le composant permet de mettre à jour le contenu textuel en temps réel, ce qui est utile pour afficher des informations contextuelles ou des changements d'état dans le jeu.

---

Le composant `TextComponent` est un outil puissant pour afficher des informations textuelles dynamiques, ajoutant une couche de clarté et d'immersion au jeu.

