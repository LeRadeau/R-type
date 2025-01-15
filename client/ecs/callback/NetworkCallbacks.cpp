#include "NetworkCallbacks.hpp"
#include <iostream>
#include "Serializer.hpp"
#include "ecs/component/TextComponent.hpp"

namespace NetworkCallbacks
{
    void onWaitUpdateClientNbr(const char *&packet, Entity &textField)
    {
        size_t nbPlayers = Serializer::deserialize<std::size_t>(packet);
        std::cout << nbPlayers << std::endl;
        auto *textComponent = textField.getComponent<TextComponent>();
        textComponent->data.setString(std::to_string(nbPlayers) + "/4 players");
    }
} // namespace NetworkCallbacks
