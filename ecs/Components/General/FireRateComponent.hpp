#pragma once

#include "../Component.hpp"

struct FireRateComponent : public Component {
    float fireRate;
    float elapsedTime;

    FireRateComponent(float fireRate = 0.5f)
        : fireRate(fireRate), elapsedTime(0.0f) {}
};
