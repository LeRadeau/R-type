#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <iostream>

#include "AComponent.hpp"

struct SoundComponent : public AComponent {
    sf::Sound sound;
    sf::SoundBuffer buffer;
    int timesPlayed;
    int maxTimesPlayed;
    SoundComponent(const std::string &filename, int maxTimesPlayed) : timesPlayed(0), maxTimesPlayed(maxTimesPlayed)
    {
        if (!buffer.loadFromFile(filename)) {
            std::cerr << "Error while loading sound: " << filename << " not found" << std::endl;
        }
        sound.setBuffer(buffer);
        sound.setVolume(100);
    }
};
