#pragma once
#include "AComponent.hpp"

struct InputComponent : public AComponent {
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    bool spaceBar = false;
};
