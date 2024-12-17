#pragma once

#include <cstdint>

enum class MessageType : uint8_t {
    CONNECT = 0,
    GOODBYE = 1,
    MOVE = 2,
    UPDATE_CLIENTS = 3,
    ERROR = 4
};
