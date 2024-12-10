#pragma once

#include "../Component.hpp"

struct DamageComponent : public Component {
    int damageAmount;

    DamageComponent(int damageAmount = 10) : damageAmount(damageAmount) {}
};
