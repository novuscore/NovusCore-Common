#pragma once
#include "BaseSocket.h"

class Connection
{
public:
    using tcp = asio::ip::tcp;
    Connection(tcp::socket* socket, u64 identity = 0) :  _identity(identity)
    {
        _baseSocket = new BaseSocket(socket, std::bind(&Connection::HandleRead, this), std::bind(&Connection::HandleDisconnect, this));
    }

    void Listen();
    void Connect(tcp::endpoint endpoint);
    void Connect(std::string address, u16 port);
    void HandleConnect();
    void HandleDisconnect();
    void HandleRead();
    void Send(ByteBuffer* buffer) { _baseSocket->Send(buffer); }

    BaseSocket* GetBaseSocket() { return _baseSocket; }
    bool HasIdentity() { return _identity; }
    u64 GetIdentity() { return _identity; }
    void SetIdentity(u64 identity) { _identity = identity; }

private:
    BaseSocket* _baseSocket;
    u64 _identity;
};