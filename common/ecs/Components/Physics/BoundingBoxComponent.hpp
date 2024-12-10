#pragma once

#include "../Component.hpp"

struct BoundingBoxComponent : public Component {
    float width, height;
    BoundingBoxComponent(float width = 0, float height = 0)
        : width(width), height(height) {}
};
