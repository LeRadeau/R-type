# SoundComponent

## Introduction

Le composant `SoundComponent` est utilisé pour gérer les effets sonores associés à une entité. Il permet de jouer des sons, de les configurer et de contrôler leur comportement, comme le volume et la répétition.

## Attributs

- **sound** : Un objet `sf::Sound` pour jouer le son.
- **buffer** : Un objet `sf::SoundBuffer` qui contient les données audio.
- **maxTimesPlayed** : Nombre maximum de fois que le son peut être joué (-1 pour une boucle infinie).
- **timesPlayed** : Nombre de fois que le son a été joué.

### Exemple d'attributs dans le fichier `SoundComponent.hpp` :

```cpp
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class SoundComponent : public AComponent {
public:
    sf::Sound sound;
    sf::SoundBuffer buffer;
    int maxTimesPlayed;
    int timesPlayed;

    SoundComponent(const std::string &soundPath, int maxTimesPlayed)
        : maxTimesPlayed(maxTimesPlayed), timesPlayed(0) {
        if (!buffer.loadFromFile(soundPath)) {
            throw std::runtime_error("Failed to load sound: " + soundPath);
        }
        sound.setBuffer(buffer);
    }
};
```

## Rôle dans l'ECS

`SoundComponent` est utilisé pour :

- Ajouter des effets sonores aux entités.
- Gérer le comportement des sons, comme leur volume et leur répétition.
- Synchroniser les sons avec des événements du jeu.

## Exemples d'Utilisation

1. **Ajout à une entité** :
   ```cpp
   Entity &explosion = entityManager.createEntity();
   explosion.addComponent<SoundComponent>("assets/explosion.wav", 1);
   ```

2. **Configuration du volume** :
   ```cpp
   auto *sound = entity.getComponent<SoundComponent>();
   if (sound) {
       sound->sound.setVolume(50.0f);
   }
   ```

3. **Lecture du son** :
   ```cpp
   auto *sound = entity.getComponent<SoundComponent>();
   if (sound) {
       sound->sound.play();
   }
   ```

## Interactions

- **Avec SoundSystem** : Le système contrôle la lecture des sons en fonction des composants attachés aux entités.
- **Avec EventHandlerComponent** : Peut déclencher des sons en réponse à des événements spécifiques.

## Fonctionnalités principales

### Gestion des sons en boucle

Le composant prend en charge les sons joués en boucle infinie ou un nombre limité de fois. Il utilise `maxTimesPlayed` pour définir la durée de lecture.

---

Le composant `SoundComponent` enrichit l'expérience utilisateur en ajoutant une couche audio dynamique et immersive aux interactions du jeu.

