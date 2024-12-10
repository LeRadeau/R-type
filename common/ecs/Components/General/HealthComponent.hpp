#pragma once

#include "../Component.hpp"

struct HealthComponent : public Component {
    int health;
    HealthComponent(int health = 100) : health(health) {}
};
