#pragma once
#include "../../NovusTypes.h"

enum class ConnectionStatus : u8
{
    AUTH_NONE,
    AUTH_CHALLENGE,
    AUTH_HANDSHAKE,
    AUTH_FAILED,
    AUTH_SUCCESS,
    CONNECTED,
};