#include "BaseServer.h"
#include <Utils/Timer.h>

void BaseServer::Start()
{
    if (_isRunning)
        return;

    _isRunning = true;
    runThread = std::thread(&BaseServer::Run, this);

    Listen();
}
void BaseServer::Stop()
{
    if (!_isRunning)
        return;

    _isRunning = false;
    _acceptor.close();
}
void BaseServer::Listen()
{
    tcp::socket* socket = new tcp::socket(_ioService);
    _acceptor.async_accept(*socket, std::bind(&BaseServer::HandleNewConnection, this, socket, std::placeholders::_1));
}
void BaseServer::Run()
{
    const f32 targetDelta = 5;

    Timer timer;
    while (_isRunning)
    {
        timer.Tick();

        _mutex.lock();
        if (_connections.size())
        {
            _connections.erase(
                std::remove_if(_connections.begin(), _connections.end(), [](std::shared_ptr<Connection>& connection) 
                    {
                        return connection->GetBaseSocket()->IsClosed();
                    }),
                _connections.end());
        }
        _mutex.unlock();

        f32 deltaTime = timer.GetDeltaTime();
        if (deltaTime <= targetDelta)
        {
            i32 timeToSleep = Math::FloorToInt(targetDelta - deltaTime);
            std::this_thread::sleep_for(std::chrono::seconds(timeToSleep));
        }
    }
}
void BaseServer::HandleNewConnection(tcp::socket* socket, const asio::error_code& error)
{
    if (!error)
    {
        _mutex.lock();
        {
            socket->non_blocking(true);
            socket->set_option(asio::socket_base::send_buffer_size(4096));
            socket->set_option(asio::socket_base::receive_buffer_size(4096));
            socket->set_option(tcp::no_delay(true));

            std::shared_ptr<Connection> connection = std::make_shared<Connection>(socket);
            connection->SetInternal(_isInternal);
            connection->HandleConnect();
            _connections.push_back(connection);
        }
        _mutex.unlock();
    }

    Listen();
}