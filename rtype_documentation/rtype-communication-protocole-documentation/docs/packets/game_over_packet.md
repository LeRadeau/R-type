# GameOverPacket

## Introduction

`GameOverPacket` est utilisé pour signaler la fin de la partie. Ce paquet informe les clients que le jeu est terminé, généralement accompagné d'informations telles que le score final ou le vainqueur.

## Structure de la classe

### Attributs

- **winnerId** : (Optionnel) Identifiant du joueur ou de l'équipe gagnante.
- **finalScores** : Une liste des scores finaux des joueurs.

### Déclaration de la classe

```cpp
class GameOverPacket : public Packet {
private:
    std::string winnerId;
    std::map<std::string, int> finalScores;

public:
    GameOverPacket() = default;
    GameOverPacket(const std::string &winnerId, const std::map<std::string, int> &finalScores)
        : winnerId(winnerId), finalScores(finalScores) {}

    void serialize(std::ostream &os) const override {
        os << winnerId << ' ' << finalScores.size() << ' ';
        for (const auto &score : finalScores) {
            os << score.first << ' ' << score.second << ' ';
        }
    }

    void deserialize(std::istream &is) override {
        size_t scoreCount;
        is >> winnerId >> scoreCount;
        for (size_t i = 0; i < scoreCount; ++i) {
            std::string playerId;
            int score;
            is >> playerId >> score;
            finalScores[playerId] = score;
        }
    }

    PacketType getType() const override {
        return PacketType::GameOver;
    }

    const std::string &getWinnerId() const { return winnerId; }
    const std::map<std::string, int> &getFinalScores() const { return finalScores; }
};
```

## Utilisation

### Envoi d'un GameOverPacket

Le serveur envoie ce paquet à tous les clients à la fin de la partie.

```cpp
std::map<std::string, int> scores = {{"player1", 500}, {"player2", 300}};
GameOverPacket gameOverPacket("player1", scores);
networkManager.sendPacket(gameOverPacket, clientIp, clientPort);
```

### Réception et traitement

Les clients affichent les informations finales de la partie.

```cpp
if (packet->getType() == PacketType::GameOver) {
    auto gameOverPacket = dynamic_cast<GameOverPacket *>(packet.get());
    std::cout << "Gagnant : " << gameOverPacket->getWinnerId() << std::endl;
    for (const auto &score : gameOverPacket->getFinalScores()) {
        std::cout << score.first << " : " << score.second << " points" << std::endl;
    }
}
```

## Interactions

- **Avec GameState** : Met à jour l'état pour refléter la fin de la partie.
- **Avec RenderSystem** : Affiche les scores finaux et les informations de victoire.

## Points forts

- **Flexibilité** : Inclut les scores finaux et le vainqueur de manière optionnelle.
- **Communication claire** : Informe tous les clients de la fin de la partie en temps réel.
- **Extensibilité** : Peut être étendu pour inclure d'autres métriques de fin de partie.

---

`GameOverPacket` est un élément clé pour gérer la transition vers l'écran de fin de partie, offrant des informations claires et concises sur les résultats.

