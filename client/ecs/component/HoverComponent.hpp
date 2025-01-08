#pragma once

#include "ecs/component/AComponent.hpp"

struct HoverComponent : public AComponent {
    bool isHover;
    float opacity;
    HoverComponent(float opacity = 0.8) : isHover(false), opacity(opacity)
    {
    }
};
