#pragma once
#include "NetworkClient.h"
#include <NovusTypes.h>
#include <Utils/Message.h>
#include <Utils/DebugHandler.h>
#include <Networking/NetworkPacket.h>

void NetworkClient::Listen()
{
    AsyncRead();
}
bool NetworkClient::Connect(tcp::endpoint endpoint)
{
    try
    {
        socket()->connect(endpoint);
        _internalConnected();
    }
    catch (std::exception e)
    {
        return false;
    }

    Listen();
    return true;
}
bool NetworkClient::Connect(std::string address, u16 port)
{
    return Connect(tcp::endpoint(asio::ip::address::from_string(address), port));
}