#pragma once

#include "../Component.hpp"

struct AmmoComponent : public Component {
    int ammoCount;
    int maxAmmo;

    AmmoComponent(int ammoCount = 10, int maxAmmo = 10)
        : ammoCount(ammoCount), maxAmmo(maxAmmo) {}
};
