#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/components/Transform/PositionComponent.hpp"
#include "../common/ecs/components/Network/BindRemoteComponent.hpp"

#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/NetworkManager.hpp"

#include <bits/stdc++.h>

int main()
{
    EntityManager entityManager;

    NetworkManager networkSystem;
    networkSystem.bindSocket("127.0.0.1", 8089);

    entityManager.createEntity();
    auto& window = entityManager.createEntity();
    window.addComponent<WindowComponent>(1920, 1080, "Bilux");

    auto& player = entityManager.createEntity();
    player.addComponent<InputComponent>();
    player.addComponent<PositionComponent>(10, 19);
    player.addComponent<BindRemoteComponent>();
    while (1) {
        networkSystem.receiveMessages(true);
    }
}