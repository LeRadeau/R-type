#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/components/Transform/PositionComponent.hpp"
#include "../common/ecs/components/Network/BindRemoteComponent.hpp"

#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/NetworkManager.hpp"
#include "../common/ecs/systems/Network/ServerNetwork.hpp"

#include <bits/stdc++.h>

int main()
{
    EntityManager entityManager;
    RenderSystem renderSystem;

    ServerNetworkSystem serverNetworkSystem("127.0.0.1", 8089, 0.02);

    auto& window = entityManager.createEntity();
    window.addComponent<WindowComponent>(1920, 1080, "Bilux");

    auto& player = entityManager.createEntity();
    player.addComponent<InputComponent>();
    player.addComponent<PositionComponent>(10, 19);
    player.addComponent<RenderComponent>("../common/haiMad.png", true);
    player.addComponent<BindClientComponent>();
    renderSystem.createWindow(entityManager);
    sf::RenderWindow& win = renderSystem.getWindow();
    sf::Clock clock;

    while (1) {
        float dt = clock.restart().asSeconds();
        serverNetworkSystem.dataToClients(entityManager, dt);
        serverNetworkSystem.dataFromClients(entityManager);
        win.clear();
        renderSystem.update(entityManager);
        win.display();
    }
}