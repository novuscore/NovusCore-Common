#pragma once
#include <robin_hood.h>
#include <memory>
#include "../Networking/ConnectionStatus.h"
#include "../Networking/Opcode.h"

class NetworkClient;
struct NetworkPacket;

typedef bool (*MessageHandlerFn)(std::shared_ptr<NetworkClient>, NetworkPacket*);
struct OpcodeHandler
{
    OpcodeHandler() { }
    OpcodeHandler(ConnectionStatus inStatus, u16 inMinSize, MessageHandlerFn inHandler) :status(inStatus), minSize(inMinSize), handler(inHandler) { }

    ConnectionStatus status = ConnectionStatus::AUTH_NONE;
    u16 minSize = 0;
    MessageHandlerFn handler = nullptr;
};

class MessageHandler
{
public:
    MessageHandler();

    void SetMessageHandler(Opcode opcode, OpcodeHandler handler);
    bool CallHandler(std::shared_ptr<NetworkClient> connection, NetworkPacket* packet);

private:
    OpcodeHandler handlers[static_cast<u16>(Opcode::MAX_COUNT)];
};