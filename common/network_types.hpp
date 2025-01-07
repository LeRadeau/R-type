#pragma once

#include <cstdint>

enum class MessageType : uint8_t {
        // CLIENT -> SERVER
        CONNECT = 0,
        GOODBYE = 1,
        MOVE = 2,
        SHOOT = 5,
        // SERVER -> CLIENT
        UPDATE_CLIENTS = 3,
        UPDATE_BULLETS = 6,
        UPDATE_ENEMIES = 7,
        DESTROY_BULLET = 8,
        // ERROR
        ERROR = 4
    };
