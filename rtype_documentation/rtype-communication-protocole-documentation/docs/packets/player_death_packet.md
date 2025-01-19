# PlayerDeathPacket

## Introduction

`PlayerDeathPacket` est utilisé pour notifier la mort d'un joueur dans le jeu multijoueur. Ce paquet est essentiel pour synchroniser l'état des entités entre le client et le serveur et pour déclencher des événements visuels ou sonores relatifs à la mort du joueur.

## Structure de la classe

### Attributs

- **playerId** : Identifiant unique du joueur mort.
- **killerId** : (Optionnel) Identifiant de l'entité (joueur, ennemi ou autre) ayant tué ce joueur.

### Déclaration de la classe

```cpp
class PlayerDeathPacket : public Packet {
private:
    std::string playerId;
    std::string killerId;

public:
    PlayerDeathPacket() = default;
    PlayerDeathPacket(const std::string &playerId, const std::string &killerId = "")
        : playerId(playerId), killerId(killerId) {}

    void serialize(std::ostream &os) const override {
        os << playerId << ' ' << killerId << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> playerId >> killerId;
    }

    PacketType getType() const override {
        return PacketType::PlayerDeath;
    }

    const std::string &getPlayerId() const { return playerId; }
    const std::string &getKillerId() const { return killerId; }
};
```

## Utilisation

### Envoi d'un PlayerDeathPacket

Le serveur envoie ce paquet lorsqu'un joueur est éliminé.

```cpp
PlayerDeathPacket deathPacket("player1", "enemy2");
networkManager.sendPacket(deathPacket, clientIp, clientPort);
```

### Réception et traitement

Les clients mettent à jour l'état pour refléter la mort du joueur et déclenchent les effets correspondants.

```cpp
if (packet->getType() == PacketType::PlayerDeath) {
    auto deathPacket = dynamic_cast<PlayerDeathPacket *>(packet.get());
    gameState.markPlayerAsDead(deathPacket->getPlayerId());
    std::cout << "Joueur " << deathPacket->getPlayerId()
              << " tué par " << deathPacket->getKillerId() << std::endl;
}
```

## Interactions

- **Avec GameState** : Marque le joueur comme mort et met à jour l'état global du jeu.
- **Avec RenderSystem** : Affiche des animations ou des effets visuels liés à la mort du joueur.
- **Avec SoundSystem** : Joue des sons pour indiquer la mort du joueur.

## Points forts

- **Synchronisation** : Assure que tous les clients sont informés des morts de joueurs en temps réel.
- **Flexibilité** : Permet de transmettre des informations sur l'entité ayant causé la mort.
- **Simplicité** : Interface claire pour gérer les événements liés à la mort des joueurs.

---

`PlayerDeathPacket` est crucial pour maintenir une cohérence dans les interactions multijoueurs et offrir une expérience immersive et dynamique.

