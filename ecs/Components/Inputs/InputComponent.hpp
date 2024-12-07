#pragma once

#include "../Component.hpp"

struct InputComponent : public Component {
    bool moveUp = false;
    bool moveDown = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool spaceBar = false;
};
