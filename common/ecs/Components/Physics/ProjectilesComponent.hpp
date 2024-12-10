#pragma once

#include "../Component.hpp"

struct ProjectileComponent : public Component {
    float speed;
    float lifetime;
    float distance;

    ProjectileComponent(float speed = 10.0f, float lifetime = 5.0f, float distance = 0.0f)
        : speed(speed), lifetime(lifetime), distance(distance) {}
};
