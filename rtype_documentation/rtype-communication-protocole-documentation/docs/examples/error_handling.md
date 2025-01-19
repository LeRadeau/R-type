# Error Handling

## Introduction

La gestion des erreurs est essentielle pour assurer la stabilité et la robustesse des communications réseau dans un environnement multijoueur. Ce module permet de détecter, gérer et corriger les anomalies tout en maintenant l'expérience utilisateur.

## Types d'erreurs courantes

1. **Erreurs de connexion** :
   - Échec de connexion au serveur.
   - Déconnexion inattendue d'un client.

2. **Erreurs réseau** :
   - Perte de paquets.
   - Retards importants dans la transmission (latence).

3. **Erreurs de message** :
   - Messages mal formés ou corrompus.
   - Types de messages non reconnus.

4. **Erreurs internes** :
   - Incohérences dans l'état du jeu.
   - Problèmes dans le traitement des paquets.

## Gestion des erreurs côté client

### Réception des messages d'erreur

Le serveur peut envoyer un `ErrorPacket` pour notifier le client d'un problème.

```cpp
if (packet->getType() == PacketType::Error) {
    auto errorPacket = dynamic_cast<ErrorPacket *>(packet.get());
    std::cerr << "Erreur reçue : [" << errorPacket->getErrorCode()
              << "] " << errorPacket->getErrorMessage() << std::endl;
}
```

### Tentatives de reconnexion

En cas de déconnexion, le client peut tenter une reconnexion automatique :

```cpp
if (socket.connect(serverIp, serverPort) != sf::Socket::Done) {
    std::cerr << "Impossible de se reconnecter au serveur." << std::endl;
} else {
    std::cout << "Reconnexion réussie." << std::endl;
}
```

### Validation des messages reçus

Chaque message doit être validé avant traitement :

```cpp
if (messageType != PacketType::ValidType) {
    std::cerr << "Message invalide reçu." << std::endl;
    return;
}
```

## Gestion des erreurs côté serveur

### Gestion des déconnexions

Le serveur détecte les déconnexions et met à jour l'état global.

```cpp
void handleDisconnection(const std::string &playerId) {
    gameState.removePlayer(playerId);
    broadcastPlayerDisconnect(playerId);
    std::cout << "Joueur déconnecté : " << playerId << std::endl;
}
```

### Vérification des paquets

Le serveur valide chaque paquet avant traitement :

```cpp
if (packet.size() < expectedSize) {
    std::cerr << "Données incomplètes reçues." << std::endl;
    return;
}
```

### Envoi de messages d'erreur

En cas de problème critique, le serveur informe le client via un `ErrorPacket` :

```cpp
void sendError(const Client &client, const std::string &message) {
    ErrorPacket errorPacket(500, message);
    networkManager.sendPacket(errorPacket, client.getIp(), client.getPort());
}
```

## Stratégies générales

1. **Journalisation** :
   - Tous les événements d'erreur sont enregistrés pour faciliter le débogage.

2. **Tolérance aux erreurs** :
   - Le système continue de fonctionner malgré des erreurs non critiques.

3. **Tentatives** :
   - Réessayer les actions critiques en cas d'échec.

4. **Notifications utilisateur** :
   - Informer les utilisateurs des erreurs et proposer des solutions (ex. : réessayer, contacter le support).

## Exemple complet : Gestion d'une déconnexion

1. **Client** : Le client détecte une déconnexion et tente de se reconnecter.
2. **Serveur** : Le serveur met à jour l'état et notifie les autres clients de la déconnexion.
3. **Client** : En cas d'échec de reconnexion, un message d'erreur est affiché.

---

La gestion des erreurs garantit la résilience et la stabilité du système, minimisant les interruptions et offrant une expérience utilisateur fluide.

