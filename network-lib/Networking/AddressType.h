#pragma once
#include "../../NovusTypes.h"

enum class AddressType : u8
{
    AUTH,
    REALM,
    WORLD,
    INSTANCE,
    CHAT,
    LOADBALANCE,
    REGION,
    COUNT,
    INVALID
};