#pragma once
#include <Networking/BaseSocket.h>
#include <Utils/DebugHandler.h>

enum BuildType
{
    Internal,
    Alpha,
    Beta,
    Release
};

// Align data perfectly
#pragma pack(push, 1)
struct ClientLogonChallenge
{
    u8 majorVersion;
    u8 patchVersion;
    u8 minorVersion;
    u8 buildType; // 0 Internal, 1 Alpha, 2 Beta, 3 Release
    u16 gameBuild;
    std::string gameName;
    std::string username;
    u8 A[256];

    std::string BuildTypeString()
    {
        std::string ret;

        switch (buildType)
        {
        case BuildType::Internal:
            ret = "Internal";
            break;
        case BuildType::Alpha:
            ret = "Alpha";
            break;
        case BuildType::Beta:
            ret = "Beta";
            break;
        case BuildType::Release:
            ret = "Release";
            break;
        }

        return ret;
    }
    u16 Serialize(std::shared_ptr<ByteBuffer> buffer, std::shared_ptr<ByteBuffer> aBuffer)
    {
        u16 size = static_cast<u16>(buffer->WrittenData);

        buffer->PutU8(majorVersion);
        buffer->PutU8(patchVersion);
        buffer->PutU8(minorVersion);
        buffer->PutU8(buildType);
        buffer->PutU16(gameBuild);
        buffer->PutString(gameName);
        buffer->PutString(username);
        buffer->PutBytes(aBuffer->GetDataPointer(), aBuffer->Size);

        return static_cast<u16>(buffer->WrittenData) - size;
    }
    void Deserialize(std::shared_ptr<ByteBuffer> buffer)
    {
        buffer->GetU8(majorVersion);
        buffer->GetU8(patchVersion);
        buffer->GetU8(minorVersion);
        buffer->GetU8(buildType);
        buffer->GetU16(gameBuild);
        buffer->GetString(gameName);
        buffer->GetString(username);
        buffer->GetBytes(A, 256);
    }
};
struct ServerLogonChallenge
{
    u8 status;
    u8 B[256];
    u8 s[4];

    u16 Serialize(std::shared_ptr<ByteBuffer> buffer)
    {
        u16 size = static_cast<u16>(buffer->WrittenData);

        buffer->PutU8(status);
        if (status == 0)
        {
            buffer->PutBytes(B, 256);
            buffer->PutBytes(s, 4);
        }

        return static_cast<u16>(buffer->WrittenData) - size;
    }
    void Deserialize(std::shared_ptr<ByteBuffer> buffer)
    {
        buffer->GetU8(status);
        if (status == 0)
        {
            buffer->GetBytes(B, sizeof(B));
            buffer->GetBytes(s, sizeof(s));
        }
    }
};
struct ClientLogonResponse
{
    u8 M1[32];

    void Deserialize(std::shared_ptr<ByteBuffer> buffer)
    {
        buffer->GetBytes(M1, sizeof(M1));
    }
    u16 Serialize(std::shared_ptr<ByteBuffer> buffer)
    {
        u16 size = static_cast<u16>(buffer->WrittenData);
        buffer->PutBytes(M1, sizeof(M1));

        return static_cast<u16>(buffer->WrittenData) - size;
    }
};
struct ServerLogonResponse
{
    u8 HAMK[32];

    u16 Serialize(std::shared_ptr<ByteBuffer> buffer)
    {
        u16 size = static_cast<u16>(buffer->WrittenData);
        buffer->PutBytes(HAMK, sizeof(HAMK));
        
        return static_cast<u16>(buffer->WrittenData) - size;
    }
    void Deserialize(std::shared_ptr<ByteBuffer> buffer)
    {
        buffer->GetBytes(HAMK, sizeof(HAMK));
    }
};
#pragma pack(pop)

class NetworkClient : public BaseSocket
{
public:
    using tcp = asio::ip::tcp;
    NetworkClient(tcp::socket* socket, u32 identity = 0) : BaseSocket(socket), _identity(identity) { }

    void Listen();
    bool Connect(tcp::endpoint endpoint);
    bool Connect(std::string address, u16 port);

    u64 GetIdentity() { return _identity; }
    void SetIdentity(u64 identity) { _identity = identity; }
private:
    u64 _identity;
};