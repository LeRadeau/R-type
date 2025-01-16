# AllyEntity

## Introduction

`AllyEntity` représente une entité alliée dans le jeu. Ce type d'entité est conçu pour interagir avec le joueur et peut afficher des informations telles que le nom d'utilisateur associé.

## Rôle dans l'ECS

`AllyEntity` est utilisé pour :

- Représenter les alliés du joueur avec des propriétés visuelles et textuelles.
- Intégrer des composants pour la position, le rendu et l'identité.

## Attributs

Les entités alliées sont créées avec les composants suivants :

- **PositionComponent** : Définit la position de l'allié.
- **SpriteComponent** : Affiche un sprite graphique pour représenter visuellement l'allié.
- **UsernameComponent** : Associe un nom d'utilisateur à l'entité.
- **TextComponent** : Affiche le nom d'utilisateur à côté ou au-dessus de l'entité.

### Exemple de création

#### Code extrait de `AllyEntity.cpp` :

```cpp
Entity &AllyEntity::createAllyEntity(
    EntityManager &entityManager, float x, float y, const std::string &username, const sf::Font &font)
{
    Entity &ally = entityManager.createEntity();
    ally.addComponent<PositionComponent>(x, y);
    ally.addComponent<SpriteComponent>("assets/Ally64x64.png", sf::Vector2f(1, 1));
    ally.addComponent<usernameComponent>(username);
    auto &text = ally.addComponent<TextComponent>(username, font, sf::Vector2f(x, y), sf::Color::White);
    text.data.setCharacterSize(24);
    return ally;
}
```

## Interactions

- **Avec RenderSystem** : Affiche l'entité alliée et son texte associé à l'écran.
- **Avec PositionComponent** : Met à jour la position de l'allié.
- **Avec UsernameComponent** : Identifie l'allié avec un nom unique.

## Exemples d'Utilisation

1. **Création d'un allié** :
   ```cpp
   sf::Font font;
   font.loadFromFile("assets/fonts/arial.ttf");
   AllyEntity::createAllyEntity(entityManager, 100.0f, 200.0f, "Ally1", font);
   ```

2. **Mise à jour de la position** :
   ```cpp
   auto *position = allyEntity.getComponent<PositionComponent>();
   if (position) {
       position->position.x += 10.0f;
   }
   ```

---

`AllyEntity` fournit une base solide pour gérer les entités alliées dans le jeu, offrant des fonctionnalités visuelles et des interactions conviviales pour le joueur.

