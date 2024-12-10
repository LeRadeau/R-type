#pragma once

#include "../Component.hpp"

struct VelocityComponent : public Component {
    float vx, vy, speed;
    VelocityComponent(float vx = 0, float vy = 0, float speed = 0) : vx(vx), vy(vy), speed(speed) {}
};
