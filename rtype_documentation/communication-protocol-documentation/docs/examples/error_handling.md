# Error Handling

## Introduction

La gestion des erreurs est une composante essentielle pour assurer la stabilité et la robustesse des communications réseau dans un environnement multijoueur. Cette section décrit les stratégies et mécanismes mis en place pour détecter, gérer et résoudre les erreurs qui peuvent survenir entre le client et le serveur.

## Types d'erreurs courantes

1. **Erreur de connexion** :
   - Déconnexion inattendue d'un client.
   - Échec lors de l'établissement de la connexion au serveur.

2. **Erreurs réseau** :
   - Perte de paquets.
   - Retards importants dans la transmission des messages (latence).

3. **Erreurs de message** :
   - Messages mal formés ou corrompus.
   - Types de messages non reconnus.

4. **Erreurs internes** :
   - Problèmes de logique dans le traitement des messages.
   - Incohérences dans l'état du jeu.

## Gestion des erreurs côté client

### Réception de messages d'erreur

Le serveur peut envoyer un message de type `MessageType::ERROR` au client en cas de problème.

#### Exemple :

```cpp
void MessageSystem::handleError(const char *&ptr) {
    std::string errorMessage = Serializer::deserialize<std::string>(ptr);
    std::cerr << "Erreur reçue : " << errorMessage << std::endl;
}
```

### Tentative de reconnexion

En cas de déconnexion, le client tente de se reconnecter automatiquement :

```cpp
if (socket.connect(serverIp, serverPort) != sf::Socket::Done) {
    std::cerr << "Impossible de se reconnecter au serveur." << std::endl;
} else {
    std::cout << "Reconnexion réussie." << std::endl;
}
```

### Validation des messages reçus

Chaque message est validé avant traitement :

```cpp
if (messageType != MessageType::VALID_TYPE) {
    std::cerr << "Message invalide reçu." << std::endl;
    return;
}
```

## Gestion des erreurs côté serveur

### Gestion des déconnexions

Le serveur surveille les connexions des clients et gère les déconnexions :

```cpp
void Server::handleDisconnection(Client &client) {
    std::cout << "Client déconnecté : " << client.id << std::endl;
    gameState.removeClient(client.id);
}
```

### Vérification des données

Les messages reçus sont vérifiés avant traitement :

```cpp
if (data.size() < expectedSize) {
    std::cerr << "Données incomplètes reçues." << std::endl;
    return;
}
```

### Envoi d'un message d'erreur

Le serveur informe les clients des erreurs critiques :

```cpp
void Server::sendError(Client &client, const std::string &errorMessage) {
    std::string buffer;
    Serializer::serialize(buffer, static_cast<uint8_t>(MessageType::ERROR));
    Serializer::serialize(buffer, errorMessage);
    socket.send(buffer.data(), buffer.size(), client.ip, client.port);
}
```

## Stratégies générales

1. **Journalisation** :
   - Tous les événements d'erreur sont enregistrés pour faciliter le débogage.

2. **Tolérance aux erreurs** :
   - Les systèmes continuent de fonctionner même en cas d'erreurs non critiques.

3. **Retentatives** :
   - En cas d'échec d'envoi ou de réception, plusieurs tentatives sont effectuées.

4. **Alertes utilisateur** :
   - Les erreurs critiques sont communiquées à l'utilisateur avec des instructions claires.

## Exemple complet : Gestion d'une déconnexion

1. Le client est déconnecté du serveur.
2. Le serveur détecte la déconnexion et met à jour l'état du jeu.
3. Le client tente automatiquement de se reconnecter.
4. Si la reconnexion échoue, le client affiche un message d'erreur.

---

La gestion des erreurs garantit une expérience utilisateur fluide et minimise les interruptions causées par des problèmes réseau ou des incohérences dans l'état du jeu.

