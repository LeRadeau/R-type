#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/ClientNetworkSystem.hpp"
#include "../common/ecs/systems/Network/Serializer.hpp"
#include "../common/ecs/systems/Transform/MovementSystem.hpp"

#include <bits/stdc++.h>


int main(void)
{
    EntityManager entityManager;
    ClientNetworkSystem clientNetworkSystem("127.0.0.1", 8089, 0.02);
    Entity& window = entityManager.createEntity(); // Nous l'a créeons ici pour le moment parce que qu'on peut pas créer d'entité de force avec un ID
    Entity& player = entityManager.createEntity(); // Nous l'a créeons ici pour le moment parce que qu'on peut pas créer d'entité de force avec un ID

    // Init window 
    bool chechk = false;

    RenderSystem renderSystem;
    InputSystem inputSystem;
    MovementSystem movementSystem;
    while (!renderSystem.createWindow(entityManager)) {
        if (!chechk) { // Send Connect packet
            clientNetworkSystem.test();
            chechk = true;
        }
        clientNetworkSystem.dataFromServer(entityManager);
        sleep(1);
    }

    renderSystem.createWindow(entityManager);
    sf::RenderWindow& win = renderSystem.getWindow();
    sf::Clock clock;

    while(win.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        clientNetworkSystem.dataToServer(entityManager, inputSystem, dt);
        clientNetworkSystem.dataFromServer(entityManager);
        win.clear();
        renderSystem.update(entityManager);
        win.display();
        inputSystem.update(entityManager, dt);
        movementSystem.update(entityManager);

    }



}