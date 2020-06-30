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
        _internalConnected(true);
    }
    catch (std::exception e)
    {
        _internalConnected(false);
        return false;
    }

    Listen();
    return true;
}
bool NetworkClient::Connect(u32 address, u16 port)
{
    return Connect(tcp::endpoint(asio::ip::address(asio::ip::address_v4(address)), port));
}
bool NetworkClient::Connect(std::string address, u16 port)
{
    return Connect(tcp::endpoint(asio::ip::address::from_string(address), port));
}