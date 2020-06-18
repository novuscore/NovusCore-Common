#pragma once
#include <NovusTypes.h>
#include <Utils/ByteBuffer.h>
#include "Opcode.h"

struct PacketHeader
{
    Opcode opcode = Opcode::INVALID;
    u16 size = 0;
    
    // Used by NetworkPacket when using a SharedPool
    void Reset()
    {
        opcode = Opcode::INVALID;
        size = 0;
    }
};