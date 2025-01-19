# TSQueue

## Introduction

`TSQueue` (Thread-Safe Queue) est une structure de données conçue pour gérer en toute sécurité des files d'attente dans un environnement multi-threadé. Elle est utilisée pour stocker et gérer les messages réseau de manière asynchrone entre différents threads du serveur et des clients.

## Fonctionnalités principales

### Ajouter un élément à la file

```cpp
void push_back(const T &item);
```
Ajoute un élément à la fin de la file.

#### Exemple :
```cpp
tsQueue.push_back(message);
```

### Récupérer et supprimer un élément de la tête

```cpp
T pop_front();
```
Récupère et supprime l'élément situé à la tête de la file. Si la file est vide, cette méthode bloque jusqu'à ce qu'un élément soit disponible.

#### Exemple :
```cpp
T message = tsQueue.pop_front();
```

### Vérifier si la file est vide

```cpp
bool empty() const;
```
Renvoie `true` si la file est vide.

#### Exemple :
```cpp
if (tsQueue.empty()) {
    std::cout << "Aucun message en attente." << std::endl;
}
```

### Obtenir la taille de la file

```cpp
size_t size() const;
```
Renvoie le nombre d'éléments actuellement dans la file.

#### Exemple :
```cpp
std::cout << "Messages en attente : " << tsQueue.size() << std::endl;
```

## Synchronisation multi-threadée

`TSQueue` utilise des mécanismes de synchronisation comme `std::mutex` et `std::condition_variable` pour garantir la sécurité des threads :

- **Verrouillage des opérations** : Les méthodes `push_back` et `pop_front` utilisent des verrous pour empêcher les accès concurrents.
- **Blocage conditionnel** : La méthode `pop_front` bloque si la file est vide, en attendant qu'un élément soit ajouté.

### Exemple de blocage :
```cpp
std::unique_lock<std::mutex> lock(muxQueue);
cvBlocking.wait(lock, [this]() { return !queue.empty(); });
```

## Exemple de flux

### Producteur-consommateur

#### Producteur :
```cpp
void producer(TSQueue<std::string> &queue) {
    queue.push_back("Message 1");
    queue.push_back("Message 2");
}
```

#### Consommateur :
```cpp
void consumer(TSQueue<std::string> &queue) {
    while (!queue.empty()) {
        std::string message = queue.pop_front();
        std::cout << "Message consommé : " << message << std::endl;
    }
}
```

### Utilisation avec des messages réseau

`TSQueue` est couramment utilisée pour stocker des messages entrants :

```cpp
while (socket.receive(buffer)) {
    tsQueue.push_back(buffer);
}

// Traitement des messages
while (!tsQueue.empty()) {
    auto message = tsQueue.pop_front();
    handleMessage(message);
}
```

## Points forts

- **Sécurité multi-threadée** : Conçu pour prévenir les conditions de concurrence.
- **Blocage conditionnel** : Permet aux consommateurs d'attendre l'arrivée de nouveaux éléments.
- **Facilité d'utilisation** : Offre une interface simple pour les opérations courantes.

---

`TSQueue` est un composant essentiel pour gérer les communications réseau dans un environnement multi-threadé, garantissant une coordination fluide et fiable entre les threads.

