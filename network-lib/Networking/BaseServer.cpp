#include "BaseServer.h"
#include <Utils/Timer.h>

void BaseServer::Start()
{
    if (_isRunning)
        return;

    _isRunning = true;
    runThread = std::thread(&BaseServer::Run, this);

    std::unique_ptr<tcp::socket> socket = std::make_unique<tcp::socket>(tcp::socket(_ioService));
    _acceptor.async_accept(*socket, std::bind(&BaseServer::HandleNewConnection, this, socket.get(), std::placeholders::_1));
}
void BaseServer::Stop()
{
    if (!_isRunning)
        return;

    _isRunning = false;
    _acceptor.close();
}
void BaseServer::Run()
{
    Timer timer;
    f32 targetDelta = 1.0f / 0.2f;
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

        f32 deltaTime = Math::Clamp(timer.GetDeltaTime(), 0, 5);
        i32 timeToSleep = Math::FloorToInt(5 - deltaTime);
        std::this_thread::sleep_for(std::chrono::seconds(timeToSleep));
    }
}
void BaseServer::HandleNewConnection(tcp::socket* socket, const asio::error_code& error)
{
    if (error)
        return;

    _mutex.lock();
    {
        socket->non_blocking(true);
        socket->set_option(asio::socket_base::send_buffer_size(4096));
        socket->set_option(asio::socket_base::receive_buffer_size(4096));
        socket->set_option(asio::ip::tcp::no_delay(true));

        std::shared_ptr<Connection> connection = std::make_shared<Connection>(socket);
        _connections.push_back(connection);
    }
    _mutex.unlock();
}