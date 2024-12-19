#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include <SFML/Graphics.hpp>

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
        sf::RenderWindow& getWindow() {
            return window;
        };
        void update(EntityManager& entityManager) {
            for (auto& entity : entityManager.entities) {
                auto* position = entity->getComponent<PositionComponent>();
                auto* render = entity->getComponent<RenderComponent>();
                
                if (position && render) {
                    render->sprite.setPosition(position->position.x, position->position.y);
                    window.draw(render->sprite);
                    return;
                }
                if (render) {
                    render->sprite.setPosition(0, 0);
                    window.draw(render->sprite);
                    return; 
                }
            }
        }
    protected:

    private:
        sf::RenderWindow window; // Best way if the window variable would be in WindowComponent ?
};