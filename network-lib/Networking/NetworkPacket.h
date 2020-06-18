#pragma once
#include <NovusTypes.h>
#include <Utils/ByteBuffer.h>
#include <Utils/SharedPool.h>
#include <Networking/BaseSocket.h>
#include <Networking/PacketHeader.h>

struct NetworkPacket
{
    PacketHeader header;
    std::shared_ptr<ByteBuffer> payload = nullptr;

    static std::shared_ptr<NetworkPacket> Borrow()
    {
        if (_networkPacket.empty())
        {
            NetworkPacket* newNetworkPacket = new NetworkPacket();
            _networkPacket.add(std::unique_ptr<NetworkPacket>(newNetworkPacket));
        }

        std::shared_ptr<NetworkPacket> buffer = _networkPacket.acquire();

        buffer->header.Reset();
        if (buffer->payload)
            buffer->payload = nullptr;

        return buffer;
    }

    static SharedPool<NetworkPacket> _networkPacket;
};