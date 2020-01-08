#pragma once
#include <NovusTypes.h>
#include "Connection.h"
#include <Utils/Message.h>
#include <Utils/DebugHandler.h>

#include "InputQueue.h"
#include "NetPacket.h"

void Connection::Connect(tcp::endpoint endpoint)
{
    try
    {
        _baseSocket->socket()->connect(endpoint);
    }
    catch (std::exception e)
    {
        Message packetMessage;
        packetMessage.code = MSG_IN_PRINT;
        packetMessage.message = new std::string("Failed to connect to host");

        InputQueue::PassMessage(packetMessage);
    }
    _baseSocket->AsyncRead();
}
void Connection::Connect(std::string address, u16 port)
{
    try
    {
        _baseSocket->socket()->connect(tcp::endpoint(asio::ip::address::from_string(address), port));
    }
    catch (std::exception e)
    {

        Message packetMessage;
        packetMessage.code = MSG_IN_PRINT;
        packetMessage.message = new std::string("Failed to connect to host");

        InputQueue::PassMessage(packetMessage);
    }
    _baseSocket->AsyncRead();
}

void Connection::HandleRead()
{
    std::shared_ptr<ByteBuffer> buffer = _baseSocket->GetReceiveBuffer();

    while (buffer->GetActiveSize())
    {
        u32 opcode = 0;
        u16 size = 0;

        buffer->GetU32(opcode);
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
        netPacket->data->IsOwner = true;
        std::memcpy(netPacket->data->GetInternalData(), buffer->GetReadPointer(), size);
        buffer->ReadData += size;

        Message packetMessage;
        packetMessage.code = MSG_IN_NET_PACKET;
        packetMessage.objects.push_back(this);
        packetMessage.objects.push_back(netPacket);

        InputQueue::PassMessage(packetMessage);
    }

    _baseSocket->AsyncRead();
}