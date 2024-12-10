#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

struct AnimationComponent : public Component {
    sf::Texture texture;          // Texture contenant toutes les frames de l'animation
    sf::IntRect frameRect;        // Rectangle définissant la frame actuelle
    int frameCount;               // Nombre total de frames dans l'animation
    int currentFrame;             // Index de la frame actuelle
    float timePerFrame;           // Temps d'affichage de chaque frame
    float elapsedTime;            // Temps écoulé depuis le dernier changement de frame

    AnimationComponent(const sf::Texture& texture, int frameWidth, int frameHeight, int frameCount, float timePerFrame)
        : texture(texture),
          frameRect(0, 0, frameWidth, frameHeight),
          frameCount(frameCount),
          currentFrame(0),
          timePerFrame(timePerFrame),
          elapsedTime(0.0f) {}
};