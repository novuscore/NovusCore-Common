#pragma once
#include <NovusTypes.h>
#include <Utils/ByteBuffer.h>

struct PacketHeader
{
    u16 opcode = 0;
    u16 size = 0;
};