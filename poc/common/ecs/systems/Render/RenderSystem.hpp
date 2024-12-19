#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"

class RenderSystem
{
    public:
        bool createWindow(EntityManager& em) {
            for (std::unique_ptr<Entity>& entity: em.entities) {
                WindowComponent *winCmpnt = entity->getComponent<WindowComponent>();
                if (winCmpnt) {
                    window.create(sf::VideoMode(winCmpnt->modeWidth, winCmpnt->modeHeight), winCmpnt->WindowName);
                    return true;
                }
            }
            // std::cout << "Error: RenderSystem impossible to create Window" << std::endl;
            return false;
        }
        sf::Window& getWindow() {
            return window;
        };
    protected:

    private:
        sf::Window window; // Best way if the window variable would be in WindowComponent ?
};