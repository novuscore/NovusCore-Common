#include "MessageHandler.h"
#include <NovusTypes.h>
#include "../Networking/NetworkPacket.h"

MessageHandler::MessageHandler()
{
    for (i32 i = 0; i < static_cast<u16>(Opcode::MAX_COUNT); i++)
    {
        handlers[i] = nullptr;
    }
}

void MessageHandler::SetMessageHandler(Opcode opcode, MessageHandlerFn func)
{
    handlers[static_cast<u16>(opcode)] = func;
}

bool MessageHandler::CallHandler(std::shared_ptr<NetworkClient> connection, NetworkPacket* packet)
{
    if (packet->header.opcode <= Opcode::INVALID || packet->header.opcode > Opcode::MAX_COUNT)
        return false;

    return handlers[static_cast<u16>(packet->header.opcode)] ? handlers[static_cast<u16>(packet->header.opcode)](connection, packet) : false;
}