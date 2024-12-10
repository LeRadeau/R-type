#pragma once

#include "../Component.hpp"

struct RangeComponent : public Component {
    float range;

    RangeComponent(float range = 100.0f) : range(range) {}
};
