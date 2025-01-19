# PlayersUpdatePacket

## Introduction

`PlayersUpdatePacket` est utilisé pour synchroniser les états des joueurs dans un environnement multijoueur. Ce paquet transmet les informations telles que les positions, vitesses, et scores de tous les joueurs connectés au jeu.

## Structure de la classe

### Attributs

- **playerData** : Une liste contenant les informations des joueurs, incluant :
  - `playerId` : Identifiant unique du joueur.
  - `position` : Position actuelle du joueur.
  - `velocity` : Vitesse actuelle du joueur.
  - `score` : Score actuel du joueur.

### Déclaration de la classe

```cpp
class PlayersUpdatePacket : public Packet {
private:
    struct PlayerInfo {
        std::string playerId;
        sf::Vector2f position;
        sf::Vector2f velocity;
        int score;
    };

    std::vector<PlayerInfo> playerData;

public:
    PlayersUpdatePacket() = default;

    void serialize(std::ostream &os) const override {
        os << playerData.size() << ' ';
        for (const auto &player : playerData) {
            os << player.playerId << ' '
               << player.position.x << ' ' << player.position.y << ' '
               << player.velocity.x << ' ' << player.velocity.y << ' '
               << player.score << ' ';
        }
    }

    void deserialize(std::istream &is) override {
        size_t count;
        is >> count;
        playerData.resize(count);
        for (auto &player : playerData) {
            is >> player.playerId >> player.position.x >> player.position.y
               >> player.velocity.x >> player.velocity.y >> player.score;
        }
    }

    PacketType getType() const override {
        return PacketType::PlayersUpdate;
    }

    const std::vector<PlayerInfo> &getPlayerData() const { return playerData; }
};
```

## Utilisation

### Envoi d'un PlayersUpdatePacket

Le serveur envoie ce paquet pour transmettre les données des joueurs aux clients.

```cpp
PlayersUpdatePacket updatePacket;
updatePacket.addPlayer("player1", {100.0f, 200.0f}, {1.0f, 0.0f}, 50);
networkManager.sendPacket(updatePacket, clientIp, clientPort);
```

### Réception et traitement

Les clients mettent à jour les entités locales des joueurs en fonction des informations reçues.

```cpp
if (packet->getType() == PacketType::PlayersUpdate) {
    auto updatePacket = dynamic_cast<PlayersUpdatePacket *>(packet.get());
    for (const auto &player : updatePacket->getPlayerData()) {
        gameState.updatePlayer(player.playerId, player.position, player.velocity, player.score);
    }
}
```

## Interactions

- **Avec GameState** : Met à jour les positions, vitesses, et scores des joueurs.
- **Avec RenderSystem** : Affiche les joueurs à leurs positions actuelles.
- **Avec LeaderboardSystem** : Met à jour le classement en fonction des scores.

## Points forts

- **Synchronisation** : Garantit une cohérence entre le serveur et les clients.
- **Optimisation** : Regroupe les données de plusieurs joueurs en un seul paquet.
- **Flexibilité** : Peut inclure des informations supplémentaires sur les joueurs si nécessaire.

---

`PlayersUpdatePacket` est essentiel pour maintenir une vue cohérente de l'état des joueurs dans un jeu multijoueur en temps réel.

