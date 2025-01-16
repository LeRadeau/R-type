#pragma once
#include <SFML/Graphics/Color.hpp>
#include "AComponent.hpp"

struct SelectionComponent : public AComponent {
    bool isSelected;

    SelectionComponent() : isSelected(false)
    {
    }
};
