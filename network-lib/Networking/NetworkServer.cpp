#include "NetworkServer.h"
#include <Utils/Timer.h>
#include <Utils/Message.h>

void NetworkServer::Start()
{
    if (_isRunning)
        return;

    _isRunning = true;
    _runThread = std::thread(&NetworkServer::Run, this);

    Listen();
}
void NetworkServer::Stop()
{
    if (!_isRunning)
        return;

    _isRunning = false;
    _acceptor.close();
}
void NetworkServer::Listen()
{
    tcp::socket* socket = new tcp::socket(*_ioService.get());
    _acceptor.async_accept(*socket, std::bind(&NetworkServer::_internalConnectionHandler, this, socket, std::placeholders::_1));
}
void NetworkServer::Run()
{
    const f32 targetDelta = 5;

    Timer timer;
    while (_isRunning)
    {
        timer.Tick();

        connectMutex.lock();
        if (_clients.size())
        {
            _clients.erase(
                std::remove_if(_clients.begin(), _clients.end(), [](std::shared_ptr<NetworkClient>& client)
                    {
                        return client->IsClosed();
                    }),
                _clients.end());
        }
        connectMutex.unlock();

        // This sacrifices percision for performance, but we don't need precision here
        f32 deltaTime = timer.GetDeltaTime();
        if (deltaTime <= targetDelta)
        {
            i32 timeToSleep = Math::FloorToInt(targetDelta - deltaTime);
            std::this_thread::sleep_for(std::chrono::seconds(timeToSleep));
        }
    }
}