#include "SoundSystem.hpp"
#include "ecs/component/SoundComponent.hpp"

void SoundSystem::update(EntityManager &entityManager, int volume)
{
    for (size_t i = 0; i < entityManager.entities.size(); i++) {
        auto &entity = entityManager.entities[i];
        auto *soundComponent = entity->getComponent<SoundComponent>();

        if (soundComponent
            && (soundComponent->timesPlayed < soundComponent->maxTimesPlayed || soundComponent->maxTimesPlayed == -1)) {
            soundComponent->sound.setVolume(volume);
            if (soundComponent->maxTimesPlayed != -1) {
                soundComponent->sound.play();
                soundComponent->timesPlayed++;
            } else {
                if (!soundComponent->sound.getLoop()) {
                    soundComponent->sound.setLoop(true);
                    soundComponent->sound.play();
                }
            }
        }
    }
}
