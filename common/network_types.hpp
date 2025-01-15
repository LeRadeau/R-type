#pragma once

#include <cstdint>

enum class MessageType : uint8_t {
        // CLIENT -> SERVER
        CONNECT = 0,
        GOODBYE = 1,
        MOVE = 2,
        SHOOT = 5,
        READY = 9, // Client informe au serveur que le jeu peux commencer
        // SERVER -> CLIENT
        UPDATE_CLIENTS = 3,
        UPDATE_BULLETS = 6,
        UPDATE_ENEMIES = 7,
        DESTROY_BULLET = 8,
        START_GAME = 10, // Le jeu commence
        WAIT = 11, // + {Nombres de joueur}
        // ERROR
        ERROR = 4,
        NONE = 12
    };
