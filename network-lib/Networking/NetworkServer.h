#pragma once
#include <NovusTypes.h>
#include <asio.hpp>
#include "NetworkClient.h"

class NetworkServer
{
public:
    using tcp = asio::ip::tcp;
    NetworkServer(std::shared_ptr<asio::io_service> ioService, i16 port) : _ioService(ioService), _acceptor(*ioService.get(), tcp::endpoint(tcp::v4(), port)), _isRunning(false)
    {
        _clients.reserve(4096);
    }

    void Start();
    void Stop();
    void Listen();
    void Run();

    void _internalConnectionHandler(tcp::socket* socket, const asio::error_code& error)
    {
        if (_connectionHandler)
            _connectionHandler(this, socket, error);

        Listen();
    }
    void SetConnectionHandler(std::function<void(NetworkServer*, tcp::socket*, const asio::error_code&)> connectionHandler)
    {
        _connectionHandler = connectionHandler;
    }

    void AddConnection(std::shared_ptr<NetworkClient> client)
    {
        connectMutex.lock();
        _clients.push_back(client);
        connectMutex.unlock();

        client->_internalConnected();
    }

    u16 GetPort() { return _acceptor.local_endpoint().port(); }
    bool IsRunning() { return _isRunning; }

    std::mutex connectMutex;
private:
    std::shared_ptr<asio::io_service> _ioService;
    asio::ip::tcp::acceptor _acceptor;
    std::function<void(NetworkServer*, tcp::socket*, const asio::error_code&)> _connectionHandler;

    std::thread _runThread;
    bool _isRunning;
    std::vector<std::shared_ptr<NetworkClient>> _clients;
};