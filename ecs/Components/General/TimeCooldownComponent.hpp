#pragma once

#include "../Component.hpp"
#include <string>

struct TimeCooldownComponent : public Component {
    float cooldownTime; // Temps de pause en secondes
    float elapsedTime;  // Temps écoulé depuis la dernière activation

    TimeCooldownComponent(float cooldownTime = 3.0f) //3 secondes
        : cooldownTime(cooldownTime), elapsedTime(0.0f) {}
};