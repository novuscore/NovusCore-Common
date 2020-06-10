#pragma once
#include <memory>
#include "../Networking/Opcode.h"

class NetworkClient;
struct NetworkPacket;
class MessageHandler
{
typedef bool (*MessageHandlerFn)(std::shared_ptr<NetworkClient>, NetworkPacket*);

public:
    MessageHandler();

    void SetMessageHandler(Opcode opcode, MessageHandlerFn func);
    bool CallHandler(std::shared_ptr<NetworkClient> connection, NetworkPacket* packet);

private:
    MessageHandlerFn handlers[static_cast<u16>(Opcode::MAX_COUNT)];
};