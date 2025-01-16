#pragma once

#include "AComponent.hpp"

struct ScoreComponent : public AComponent {
    int score;

    ScoreComponent() : score(0)
    {
    }
};
