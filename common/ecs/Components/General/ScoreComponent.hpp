#pragma once

#include "../Component.hpp"

struct ScoreComponent : public Component {
    int score;

    ScoreComponent(int score = 0) : score(score) {}
};
