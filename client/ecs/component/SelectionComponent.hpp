#pragma once
#include "AComponent.hpp"

struct SelectionComponent : public AComponent {
    bool isSelected;
    SelectionComponent() : isSelected(false)
    {
    }
};
