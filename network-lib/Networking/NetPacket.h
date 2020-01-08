#include <NovusTypes.h>
#include <Utils\ByteBuffer.h>

struct NetPacket
{
    u32 opcode = 0;
    std::shared_ptr<ByteBuffer> data = nullptr;
};