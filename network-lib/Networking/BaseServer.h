#pragma once

#include <asio.hpp>
#include "Connection.h"

class BaseServer
{
public:
    using tcp = asio::ip::tcp;
    BaseServer(std::shared_ptr<asio::io_service> ioService, i16 port, bool isInternal = false) : _ioService(ioService), _acceptor(*ioService.get(), tcp::endpoint(tcp::v4(), port)), _isRunning(false), _isInternal(isInternal)
    {
        _connections.reserve(4096);
    }

    void Start();
    void Stop();
    void Listen();
    void Run();
    void HandleNewConnection(tcp::socket* socket, const asio::error_code& error);
    void AddConnection(std::shared_ptr<Connection> connection) 
    { 
        _connections.push_back(connection); 
        connection->HandleConnect();
    }

    u16 GetPort() { return _acceptor.local_endpoint().port(); }
    bool IsRunning() { return _isRunning; }
    bool IsInternal() { return _isInternal; }
private:
    std::shared_ptr<asio::io_service> _ioService;
    asio::ip::tcp::acceptor _acceptor;

    std::mutex _mutex;
    std::thread runThread;
    bool _isRunning;
    bool _isInternal;
    std::vector<std::shared_ptr<Connection>> _connections;
};