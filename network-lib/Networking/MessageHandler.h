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
    OpcodeHandler(ConnectionStatus inStatus, u16 inMinMaxSize, MessageHandlerFn inHandler) :status(inStatus), minSize(inMinMaxSize), maxSize(inMinMaxSize), handler(inHandler) { }
    OpcodeHandler(ConnectionStatus inStatus, u16 inMinSize, i16 inMaxSize, MessageHandlerFn inHandler) :status(inStatus), minSize(inMinSize), maxSize(inMaxSize), handler(inHandler) { }

    ConnectionStatus status = ConnectionStatus::AUTH_NONE;
    u16 minSize = 0;
    i16 maxSize = 0;
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