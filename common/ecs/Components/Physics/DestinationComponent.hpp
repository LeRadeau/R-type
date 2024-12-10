#pragma once

#include "../Component.hpp"

struct DestinationComponent : public Component {
    float depx, depy;
    float desx, desy;
    bool todes = false;
    DestinationComponent(float depx = 0, float depy = 0, float desx = 0, float desy = 0) : depx(depx), depy(depy), desx(desx), desy(desy) {}
};
