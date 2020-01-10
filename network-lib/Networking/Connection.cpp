#pragma once
#include <NovusTypes.h>
#include "Connection.h"
#include <Utils/Message.h>
#include <Utils/DebugHandler.h>

#include "InputQueue.h"
#include "NetPacket.h"

void Connection::Listen()
{
    _baseSocket->AsyncRead();
}
bool Connection::Connect(tcp::endpoint endpoint)
{
    try
    {
        _baseSocket->socket()->connect(endpoint);
    }
    catch (std::exception e)
    {
        return false;
    }
    _baseSocket->AsyncRead();
    return true;
}
bool Connection::Connect(std::string address, u16 port)
{
    return Connect(tcp::endpoint(asio::ip::address::from_string(address), port));
}
void Connection::HandleConnect()
{
    Listen();
}
void Connection::HandleDisconnect()
{
    Message packetMessage;
    packetMessage.code = MSG_IN_NET_DISCONNECT;
    packetMessage.objects.push_back(new u64(_identity));

    InputQueue::PassMessage(packetMessage);
}
void Connection::HandleRead()
{
    std::shared_ptr<ByteBuffer> buffer = _baseSocket->GetReceiveBuffer();

    while (buffer->GetActiveSize())
    {
        u16 opcode = 0;
        u16 size = 0;

        buffer->GetU16(opcode);
        buffer->GetU16(size);

        if (size > 8192)
        {
            _baseSocket->Close(asio::error::shut_down);
            return;
        }

        NetPacket* netPacket = new NetPacket();
        netPacket->opcode = opcode;
        netPacket->data = ByteBuffer::Borrow<4096>();
        netPacket->data->Size = size;
        netPacket->data->WrittenData = size;
        std::memcpy(netPacket->data->GetInternalData(), buffer->GetReadPointer(), size);
        buffer->ReadData += size;

        Message packetMessage;
        packetMessage.code = IsInternal() ? InputMessages::MSG_IN_INTERNAL_NET_PACKET : InputMessages::MSG_IN_NET_PACKET;
        packetMessage.objects.push_back(this);
        packetMessage.objects.push_back(netPacket);

        InputQueue::PassMessage(packetMessage);
    }

    _baseSocket->AsyncRead();
}