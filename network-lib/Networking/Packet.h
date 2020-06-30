#pragma once
#include <NovusTypes.h>
#include <Utils/ByteBuffer.h>
#include <Networking/PacketHeader.h>
#include <Networking/Connection.h>

struct Packet
{
    PacketHeader header;
    std::shared_ptr<Bytebuffer> payload = nullptr;
    std::shared_ptr<Connection> connection;
};