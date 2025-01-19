#include "NetworkCallbacks.hpp"
#include "ecs/component/TextComponent.hpp"
#include "network/packets/Packet.hpp"
#include "network/packets/WaitingUpdatePacket.hpp"

namespace NetworkCallbacks
{
    void onWaitUpdateClientNbr(const std::shared_ptr<Network::Packet> &packet, Entity &textField)
    {
        auto waitUpdatePacket = std::dynamic_pointer_cast<Network::WaitingUpdatePacket>(packet);
        if (!waitUpdatePacket)
            return;
        auto *textComponent = textField.getComponent<TextComponent>();
        textComponent->data.setString(std::to_string(waitUpdatePacket->getClientTotal()) + "/4 players");
    }
} // namespace NetworkCallbacks
