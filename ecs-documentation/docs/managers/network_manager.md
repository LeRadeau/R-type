# NetworkManager

## Introduction

Le gestionnaire réseau, `NetworkManager`, est chargé de gérer les communications entre le client et le serveur. Il envoie et reçoit des messages pour synchroniser l'état du jeu entre les différentes instances.

## Rôle dans l'ECS

`NetworkManager` a pour rôle de :

- Envoyer des messages du client vers le serveur.
- Recevoir et traiter les messages envoyés par le serveur.
- Assurer une gestion fluide des communications réseau, même en cas d'erreurs ou de pertes de paquets.

## Fonctionnalités principales

### Constructeur `NetworkManager`

Initialise le gestionnaire réseau avec une adresse IP de serveur et un port.

#### Exemple de code

```cpp
NetworkManager::NetworkManager(const std::string &serverIp, uint16_t port) : serverIp(serverIp), serverPort(port) {
    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        throw std::runtime_error("Failed to bind socket");
    }
    receiverThread = std::thread(&NetworkManager::receiveMessages, this);
}
```

### Méthode `send`

Envoie un message au serveur.

#### Exemple de code

```cpp
void NetworkManager::send(const std::string &buffer) {
    socket.send(buffer.data(), buffer.size(), serverIp, serverPort);
}
```

### Méthode `getReceivedMessages`

Récupère les messages reçus.

#### Exemple de code

```cpp
std::queue<std::string> &NetworkManager::getReceivedMessages() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return receivedMessages;
}
```

### Méthode `receiveMessages`

Écoute les messages entrants et les stocke dans une file d'attente.

#### Exemple de code

```cpp
void NetworkManager::receiveMessages() {
    char data[1024];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short senderPort;

    while (true) {
        if (socket.receive(data, sizeof(data), received, sender, senderPort) == sf::Socket::Done) {
            std::lock_guard<std::mutex> lock(queueMutex);
            receivedMessages.push(std::string(data, received));
        }
    }
}
```

## Exemples d'Utilisation

1. **Initialisation du gestionnaire réseau** :
   ```cpp
   NetworkManager networkManager("127.0.0.1", 54000);
   ```

2. **Envoi d'un message** :
   ```cpp
   std::string message = "Hello, server!";
   networkManager.send(message);
   ```

3. **Récupération des messages reçus** :
   ```cpp
   auto &messages = networkManager.getReceivedMessages();
   while (!messages.empty()) {
       std::cout << "Message reçu : " << messages.front() << std::endl;
       messages.pop();
   }
   ```

## Interactions

- **Avec MessageSystem** : Fournit les données réseau nécessaires pour mettre à jour les entités.
- **Avec Serializer** : Encode et décode les messages pour assurer leur compatibilité avec le protocole réseau.

## Points forts

- **Multithreadé** : Gère les envois et réceptions de manière asynchrone pour éviter de bloquer le jeu.
- **Robuste** : Gère les erreurs réseau pour assurer une expérience fluide.

---

`NetworkManager` est un composant essentiel pour les jeux multijoueurs, garantissant des communications réseau fiables et efficaces entre les clients et le serveur.

