#pragma once
#include <NovusTypes.h>
#include <Utils/ByteBuffer.h>
#include "Opcode.h"

struct PacketHeader
{
    Opcode opcode = Opcode::INVALID;
    u16 size = 0;
};