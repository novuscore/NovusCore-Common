#include "MessageHandler.h"
#include <NovusTypes.h>
#include "../Networking/NetworkPacket.h"

MessageHandler::MessageHandler()
{
    for (i32 i = 0; i < Opcode::OPCODE_MAX_COUNT; i++)
    {
        handlers[i] = nullptr;
    }
}

void MessageHandler::SetMessageHandler(Opcode opcode, MessageHandlerFn func)
{
    handlers[opcode] = func;
}

bool MessageHandler::CallHandler(std::shared_ptr<NetworkClient> connection, NetworkPacket* packet)
{
    return handlers[packet->header.opcode] ? handlers[packet->header.opcode](connection, packet) : true;
}