# PlayerConnectPacket

## Introduction

`PlayerConnectPacket` est utilisé pour signaler la connexion d’un joueur au serveur. Ce paquet contient des informations essentielles telles que l’identifiant du joueur et d’autres données optionnelles associées à la connexion.

## Structure de la classe

### Attributs

- **playerId** : Identifiant unique du joueur.
- **initialData** : (Optionnel) Données supplémentaires concernant le joueur, telles que son nom ou ses statistiques initiales.

### Déclaration de la classe

```cpp
class PlayerConnectPacket : public Packet {
private:
    std::string playerId;
    std::string initialData;

public:
    PlayerConnectPacket() = default;
    PlayerConnectPacket(const std::string &playerId, const std::string &initialData = "")
        : playerId(playerId), initialData(initialData) {}

    void serialize(std::ostream &os) const override {
        os << playerId << ' ' << initialData << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> playerId;
        std::getline(is, initialData);
    }

    PacketType getType() const override {
        return PacketType::PlayerConnect;
    }

    const std::string &getPlayerId() const { return playerId; }
    const std::string &getInitialData() const { return initialData; }
};
```

## Utilisation

### Envoi d'un PlayerConnectPacket

Le client envoie ce paquet pour se connecter au serveur.

```cpp
PlayerConnectPacket connectPacket("player1", "Nom:John;Stats:50");
networkManager.sendPacket(connectPacket, serverIp, serverPort);
```

### Réception et traitement

Le serveur ajoute le joueur à la liste des entités actives et enregistre ses données initiales.

```cpp
if (packet->getType() == PacketType::PlayerConnect) {
    auto connectPacket = dynamic_cast<PlayerConnectPacket *>(packet.get());
    gameState.addPlayer(connectPacket->getPlayerId(), connectPacket->getInitialData());
    std::cout << "Joueur connecté : " << connectPacket->getPlayerId() << std::endl;
}
```

## Interactions

- **Avec GameState** : Ajoute un joueur à l'état global du jeu.
- **Avec NetworkManager** : Sérialise et transmet les informations de connexion au serveur.

## Points forts

- **Flexibilité** : Permet d'inclure des données optionnelles spécifiques à chaque joueur.
- **Simplicité** : Fournit une interface claire pour établir des connexions joueur-serveur.

---

`PlayerConnectPacket` est essentiel pour gérer les connexions des joueurs, garantissant une intégration fluide dans le système multijoueur.

