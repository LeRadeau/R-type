#include <iostream>
#include "Server.hpp"

int main()
{
    Server server(SERVER_PORT, SERVER_PORT);

    try {
        server.init();
        server.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
