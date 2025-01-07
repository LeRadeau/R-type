#pragma once
#include "AComponent.hpp"

struct HealthComponent : public AComponent {
    int health;
    HealthComponent(int health = 100) : health(health)
    {
    }
};
