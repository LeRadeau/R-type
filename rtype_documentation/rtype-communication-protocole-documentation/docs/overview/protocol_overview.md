# Protocol Overview

## Introduction

Le protocole de communication du projet R-Type définit les règles et les formats pour les échanges de données entre le serveur et les clients. Il repose sur une architecture modulaire permettant de traiter différents types de messages (paquets) de manière efficace et extensible.

## Objectifs du protocole

1. **Synchronisation en temps réel** : Assurer que tous les clients ont une vue cohérente de l'état du jeu.
2. **Gestion des événements de jeu** : Transmettre les actions des joueurs, les mises à jour des entités et les états du jeu.
3. **Tolérance aux erreurs** : Gérer les pertes de paquets ou les erreurs réseau avec des mécanismes robustes.

## Structure générale

### Composition d'un message réseau

Chaque message réseau est constitué de deux parties principales :

1. **En-tête (Header)** : Identifie le type de message et contient des métadonnées nécessaires au traitement.
   - **Type de message** : Un champ `uint8_t` spécifiant la catégorie du message (ex. : `MOVE`, `SHOOT`).
   - **Taille** : Indique la longueur totale des données.

2. **Données (Payload)** : Contient les informations spécifiques à chaque type de message.

#### Exemple de structure :

| Champ       | Type       | Description                          |
|-------------|------------|--------------------------------------|
| Type        | `uint8_t`  | Identifie le type de message.        |
| Taille      | `uint16_t` | Taille totale des données.           |
| Données     | `variable` | Contenu du message sérialisé.        |

### Cycle de vie d'un message

1. **Création** : Le client ou le serveur génère un message avec les données nécessaires.
2. **Sérialisation** : Les données sont converties en un format binaire pour la transmission.
3. **Transmission** : Le message est envoyé via le réseau (UDP).
4. **Désérialisation** : Le destinataire reconstruit les données à partir du message binaire.
5. **Traitement** : Le message est interprété et les actions correspondantes sont exécutées.

## Types de messages

Le protocole prend en charge une variété de messages, définis dans `network_types.hpp` et implémentés dans des classes spécifiques. Voici quelques exemples :

- **`PlayerConnectPacket`** : Gère la connexion initiale d'un joueur.
- **`PlayerMovePacket`** : Transmet les mouvements des joueurs.
- **`BulletsUpdatePacket`** : Met à jour les positions des projectiles.
- **`ErrorPacket`** : Notifie les erreurs réseau.
- **`GameStartPacket`** : Signale le début de la partie.

## Optimisations

1. **Compression des données** : Les paquets réseau sont réduits pour minimiser la bande passante.
2. **Agrégation des mises à jour** : Plusieurs petites mises à jour sont combinées en un seul message.
3. **Tolérance aux pertes** : Les messages critiques peuvent être réessayés si leur réception échoue.

## Exemple de flux réseau

1. Le joueur effectue une action (ex. : déplacement).
2. Un `PlayerMovePacket` est créé et envoyé au serveur.
3. Le serveur traite le message et met à jour l'état global.
4. Le serveur diffuse un `PlayersUpdatePacket` à tous les clients pour synchroniser les positions.

---

Ce protocole est conçu pour offrir une communication fiable et performante dans un environnement multijoueur compétitif.

