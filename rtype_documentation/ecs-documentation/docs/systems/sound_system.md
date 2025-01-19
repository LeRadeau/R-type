# SoundSystem

## Introduction

`SoundSystem` gère la lecture et la gestion des effets sonores dans le jeu. Il utilise les composants `SoundComponent` pour jouer des sons associés aux entités, contrôler le volume et gérer les répétitions.

## Rôle dans l'ECS

`SoundSystem` est utilisé pour :

- Contrôler les effets sonores associés aux entités.
- Gérer le volume global ou individuel des sons.
- Assurer la lecture des sons en boucle ou un nombre limité de fois.

## Fonctionnalités principales

### Méthode `update`

La méthode `update` vérifie les composants `SoundComponent` des entités et joue les sons selon leurs configurations.

#### Code extrait de `SoundSystem.cpp` :

```cpp
void SoundSystem::update(EntityManager &entityManager, int volume)
{
    for (size_t i = 0; i < entityManager.entities.size(); i++) {
        auto &entity = entityManager.entities[i];
        auto *soundComponent = entity->getComponent<SoundComponent>();

        if (soundComponent
            && (soundComponent->timesPlayed < soundComponent->maxTimesPlayed || soundComponent->maxTimesPlayed == -1)) {
            soundComponent->sound.setVolume(volume);
            if (soundComponent->maxTimesPlayed != -1) {
                soundComponent->sound.play();
                soundComponent->timesPlayed++;
            } else {
                if (!soundComponent->sound.getLoop()) {
                    soundComponent->sound.setLoop(true);
                    soundComponent->sound.play();
                }
            }
        }
    }
}
```

## Interactions

- **Avec SoundComponent** : Utilise les paramètres de volume, de boucle et de répétition pour chaque son.
- **Avec EntityManager** : Parcourt les entités et applique les effets sonores définis.

## Exemples d'Utilisation

1. **Initialisation et mise à jour** :
   ```cpp
   int volume = 50;
   soundSystem.update(entityManager, volume);
   ```

2. **Ajout d'un composant sonore** :
   ```cpp
   Entity &explosion = entityManager.createEntity();
   explosion.addComponent<SoundComponent>("assets/explosion.wav", 1);
   ```

3. **Contrôle du volume** :
   ```cpp
   auto *sound = entity.getComponent<SoundComponent>();
   if (sound) {
       sound->sound.setVolume(75.0f);
   }
   ```

## Fonctionnalités supplémentaires

- **Gestion du volume global** : Permet de contrôler le volume de tous les sons simultanément.
- **Support de la lecture en boucle** : Configure les sons pour être joués en boucle infinie si nécessaire.

---

`SoundSystem` enrichit l'expérience utilisateur en ajoutant une dimension sonore immersive, essentielle pour un gameplay captivant.

