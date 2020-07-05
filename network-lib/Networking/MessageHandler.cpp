#include "MessageHandler.h"
#include <NovusTypes.h>
#include "../Networking/NetworkClient.h"
#include "../Networking/NetworkPacket.h"

MessageHandler::MessageHandler() { }

void MessageHandler::SetMessageHandler(Opcode opcode, OpcodeHandler handler)
{
    handlers[static_cast<u16>(opcode)] = handler;
}

bool MessageHandler::CallHandler(std::shared_ptr<NetworkClient> connection, std::shared_ptr<NetworkPacket>& packet)
{
    if (packet->header.opcode <= Opcode::INVALID || packet->header.opcode > Opcode::MAX_COUNT)
        return false;

    const OpcodeHandler& opcodeHandler = handlers[static_cast<u16>(packet->header.opcode)];

    if (!opcodeHandler.handler || packet->header.size < opcodeHandler.minSize || (packet->header.size > opcodeHandler.maxSize && opcodeHandler.maxSize != -1) || connection->GetStatus() != opcodeHandler.status)
        return false;

    return opcodeHandler.handler(connection, packet);
}