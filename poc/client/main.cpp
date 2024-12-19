#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/ClientNetworkSystem.hpp"
#include "../common/ecs/systems/Network/Serializer.hpp"

#include <bits/stdc++.h>


int main(void)
{
    EntityManager entityManager;
    ClientNetworkSystem clientNetworkSystem("127.0.0.1", 8089);
    Entity& window = entityManager.createEntity(); // Nous l'a créeons ici pour le moment parce que qu'on peut pas créer d'entité de force avec un ID
    Entity& player = entityManager.createEntity(); // Nous l'a créeons ici pour le moment parce que qu'on peut pas créer d'entité de force avec un ID

    // Init window 
    bool chechk = false;

    RenderSystem rendersystem;
    while (!rendersystem.createWindow(entityManager)) {
        if (!chechk) {
            clientNetworkSystem.test();
            chechk = true;
        }
        clientNetworkSystem.dataFromServer(entityManager);
        sleep(1);
    }
    // window.addComponent<WindowComponent>(1920, 1080); // On peut créer la window directement avec le serveur, mais pour être plus rapide on peut l'a crée ici.

    rendersystem.createWindow(entityManager);
    sf::RenderWindow& win = rendersystem.getWindow();

    while(win.isOpen()) {
        if (!chechk) {
            // Le test fait en sorte d'envoyer un paquet CONNECT au serveur
            clientNetworkSystem.test();
            chechk = true;
        }
        clientNetworkSystem.dataFromServer(entityManager);
        win.clear();
        rendersystem.update(entityManager);

        win.display();
    }



}