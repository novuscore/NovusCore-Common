#pragma once

#include <asio.hpp>
#include "Connection.h"

class BaseServer
{
public:
    using tcp = asio::ip::tcp;
    BaseServer(asio::io_service& ioService, i16 port) : _ioService(ioService), _acceptor(ioService, tcp::endpoint(tcp::v4(), port))
    {
        _connections.reserve(4096);
    }

    void Start();
    void Stop();
    void Run();
    void HandleNewConnection(tcp::socket* socket, const asio::error_code& error);

    u16 GetPort() { return _acceptor.local_endpoint().port(); }
    bool IsRunning() { return _isRunning; }
private:
    asio::io_service& _ioService;
    asio::ip::tcp::acceptor _acceptor;

    std::mutex _mutex;
    std::thread runThread;
    bool _isRunning = false;
    std::vector<std::shared_ptr<Connection>> _connections;
};