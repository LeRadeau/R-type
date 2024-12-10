#include <iostream>
#include "UdpSocket.hpp"

int main()
{
    UdpSocket serv;

    serv.bindSocket("127.0.0.1", 12345);
    Packet packet("127.0.0.1", 12345, 0, Packet::Type::GAME_START, 0, nullptr);
    serv.sendTo(packet);
    std::vector<Packet> packets = serv.receive();
    for (const auto &i : packets) {
        Packet::Type type = i.getType();
        const char *to_str = type == Packet::Type::GAME_START ? "GAME_START" : "false";
        std::cout << i.getSenderIp() << ":" << to_str << std::endl;
    }
}
