#pragma once
#include "BaseSocket.h"

class Connection
{
public:
    using tcp = asio::ip::tcp;
    Connection(tcp::socket* socket, u64 identity = 0, bool isInternal = false) : _identity(identity), _isInternal(isInternal)
    {
        _baseSocket = new BaseSocket(socket, std::bind(&Connection::HandleRead, this), std::bind(&Connection::HandleDisconnect, this));
    }

    void Listen();
    bool Connect(tcp::endpoint endpoint);
    bool Connect(std::string address, u16 port);
    void HandleConnect();
    void HandleDisconnect();
    void HandleRead();
    void Send(ByteBuffer* buffer) { _baseSocket->Send(buffer); }

    BaseSocket* GetBaseSocket() { return _baseSocket; }

    u64 GetIdentity() { return _identity; }
    void SetIdentity(u64 identity) { _identity = identity; }

    bool IsInternal() { return _isInternal; }
    void SetInternal(bool state) { _isInternal = state; }

private:
    BaseSocket* _baseSocket;
    u64 _identity;
    bool _isInternal;
};