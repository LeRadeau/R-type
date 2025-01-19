# EnemiesUpdatePacket

## Introduction

`EnemiesUpdatePacket` est utilisé pour synchroniser l'état des ennemis entre le serveur et les clients. Il permet de transmettre les positions, la santé, et d'autres attributs importants des ennemis, garantissant une cohérence dans le jeu multijoueur.

## Structure de la classe

### Attributs

- **enemyData** : Une liste d'objets contenant les informations de chaque ennemi, incluant :
  - `enemyId` : Identifiant unique de l'ennemi.
  - `position` : Position actuelle de l'ennemi.
  - `health` : Points de vie actuels de l'ennemi.

### Déclaration de la classe

```cpp
class EnemiesUpdatePacket : public Packet {
private:
    struct EnemyInfo {
        std::string enemyId;
        sf::Vector2f position;
        int health;
    };

    std::vector<EnemyInfo> enemyData;

public:
    EnemiesUpdatePacket() = default;

    void serialize(std::ostream &os) const override {
        os << enemyData.size() << ' ';
        for (const auto &enemy : enemyData) {
            os << enemy.enemyId << ' '
               << enemy.position.x << ' ' << enemy.position.y << ' '
               << enemy.health << ' ';
        }
    }

    void deserialize(std::istream &is) override {
        size_t count;
        is >> count;
        enemyData.resize(count);
        for (auto &enemy : enemyData) {
            is >> enemy.enemyId >> enemy.position.x >> enemy.position.y >> enemy.health;
        }
    }

    PacketType getType() const override {
        return PacketType::EnemiesUpdate;
    }

    const std::vector<EnemyInfo> &getEnemyData() const { return enemyData; }
};
```

## Utilisation

### Envoi d'un EnemiesUpdatePacket

Le serveur envoie ce paquet pour informer les clients des positions et états actuels des ennemis.

```cpp
EnemiesUpdatePacket updatePacket;
updatePacket.addEnemy("enemy1", {300.0f, 400.0f}, 100);
networkManager.sendPacket(updatePacket, clientIp, clientPort);
```

### Réception et traitement

Les clients mettent à jour leurs entités ennemies locales avec les données reçues.

```cpp
if (packet->getType() == PacketType::EnemiesUpdate) {
    auto updatePacket = dynamic_cast<EnemiesUpdatePacket *>(packet.get());
    for (const auto &enemy : updatePacket->getEnemyData()) {
        gameState.updateEnemy(enemy.enemyId, enemy.position, enemy.health);
    }
}
```

## Interactions

- **Avec GameState** : Met à jour les attributs des ennemis dans le jeu.
- **Avec RenderSystem** : Affiche les ennemis à leurs nouvelles positions.

## Points forts

- **Optimisation** : Transmet les données de plusieurs ennemis dans un seul paquet.
- **Synchronisation** : Garantit une cohérence entre le serveur et les clients.

---

`EnemiesUpdatePacket` est un composant essentiel pour gérer efficacement les interactions et synchronisations des ennemis dans un environnement multijoueur.

