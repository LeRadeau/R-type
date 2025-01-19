# EnemyDeathPacket

## Introduction

`EnemyDeathPacket` est utilisé pour signaler la mort d'un ennemi dans le jeu multijoueur. Ce paquet informe les clients qu'un ennemi spécifique a été détruit, permettant de mettre à jour l'état du jeu et d'afficher les effets visuels ou sonores correspondants.

## Structure de la classe

### Attributs

- **enemyId** : Identifiant unique de l'ennemi détruit.
- **killerId** : (Optionnel) Identifiant de l'entité (joueur ou autre) ayant tué l'ennemi.

### Déclaration de la classe

```cpp
class EnemyDeathPacket : public Packet {
private:
    std::string enemyId;
    std::string killerId;

public:
    EnemyDeathPacket() = default;
    EnemyDeathPacket(const std::string &enemyId, const std::string &killerId)
        : enemyId(enemyId), killerId(killerId) {}

    void serialize(std::ostream &os) const override {
        os << enemyId << ' ' << killerId << ' ';
    }

    void deserialize(std::istream &is) override {
        is >> enemyId >> killerId;
    }

    PacketType getType() const override {
        return PacketType::EnemyDeath;
    }

    const std::string &getEnemyId() const { return enemyId; }
    const std::string &getKillerId() const { return killerId; }
};
```

## Utilisation

### Envoi d'un EnemyDeathPacket

Le serveur envoie ce paquet lorsqu'un ennemi est détruit pour informer les clients.

```cpp
EnemyDeathPacket deathPacket("enemy1", "player2");
networkManager.sendPacket(deathPacket, clientIp, clientPort);
```

### Réception et traitement

Les clients mettent à jour l'état du jeu pour refléter la mort de l'ennemi.

```cpp
if (packet->getType() == PacketType::EnemyDeath) {
    auto deathPacket = dynamic_cast<EnemyDeathPacket *>(packet.get());
    gameState.removeEnemy(deathPacket->getEnemyId());
    std::cout << "Ennemi " << deathPacket->getEnemyId() << " détruit par " << deathPacket->getKillerId() << std::endl;
}
```

## Interactions

- **Avec GameState** : Supprime l'ennemi de la liste des entités actives.
- **Avec RenderSystem** : Met à jour l'affichage pour refléter la mort de l'ennemi.
- **Avec SoundSystem** : Joue un son de destruction approprié.

## Points forts

- **Simplicité** : Facilite la gestion des événements liés à la destruction des ennemis.
- **Flexibilité** : Inclut des informations optionnelles sur l'entité ayant causé la destruction.
- **Synchronisation** : Garantit que tous les clients sont informés des changements d'état dans le jeu.

---

`EnemyDeathPacket` est essentiel pour gérer les interactions liées aux destructions d'ennemis, offrant une expérience multijoueur cohérente et immersive.

