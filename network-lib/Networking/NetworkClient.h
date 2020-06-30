#pragma once
#include <Networking/BaseSocket.h>
#include <Utils/DebugHandler.h>
#include <entity/fwd.hpp>

enum BuildType
{
    Internal,
    Alpha,
    Beta,
    Release
};

// Align struct data with no padding
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
    u16 Serialize(std::shared_ptr<Bytebuffer> buffer, std::shared_ptr<Bytebuffer> aBuffer)
    {
        u16 size = static_cast<u16>(buffer->writtenData);

        buffer->PutU8(majorVersion);
        buffer->PutU8(patchVersion);
        buffer->PutU8(minorVersion);
        buffer->PutU8(buildType);
        buffer->PutU16(gameBuild);
        buffer->PutString(gameName);
        buffer->PutString(username);
        buffer->PutBytes(aBuffer->GetDataPointer(), aBuffer->size);

        return static_cast<u16>(buffer->writtenData) - size;
    }
    void Deserialize(std::shared_ptr<Bytebuffer> buffer)
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

    u16 Serialize(std::shared_ptr<Bytebuffer> buffer)
    {
        u16 size = static_cast<u16>(buffer->writtenData);

        buffer->PutU8(status);
        if (status == 0)
        {
            buffer->PutBytes(B, 256);
            buffer->PutBytes(s, 4);
        }

        return static_cast<u16>(buffer->writtenData) - size;
    }
    void Deserialize(std::shared_ptr<Bytebuffer> buffer)
    {
        buffer->GetU8(status);
        if (status == 0)
        {
            buffer->GetBytes(B, sizeof(B));
            buffer->GetBytes(s, sizeof(s));
        }
    }
};
struct ClientLogonHandshake
{
    u8 M1[32];

    void Deserialize(std::shared_ptr<Bytebuffer> buffer)
    {
        buffer->GetBytes(M1, sizeof(M1));
    }
    u16 Serialize(std::shared_ptr<Bytebuffer> buffer)
    {
        u16 size = static_cast<u16>(buffer->writtenData);
        buffer->PutBytes(M1, sizeof(M1));

        return static_cast<u16>(buffer->writtenData) - size;
    }
};
struct ServerLogonHandshake
{
    u8 HAMK[32];

    u16 Serialize(std::shared_ptr<Bytebuffer> buffer)
    {
        u16 size = static_cast<u16>(buffer->writtenData);
        buffer->PutBytes(HAMK, sizeof(HAMK));
        
        return static_cast<u16>(buffer->writtenData) - size;
    }
    void Deserialize(std::shared_ptr<Bytebuffer> buffer)
    {
        buffer->GetBytes(HAMK, sizeof(HAMK));
    }
};
#pragma pack(pop)

enum class ConnectionStatus
{
    AUTH_NONE,
    AUTH_CHALLENGE,
    AUTH_HANDSHAKE,
    AUTH_FAILED,
    AUTH_SUCCESS,
    CONNECTED,
};
class NetworkClient : public BaseSocket
{
public:
    using tcp = asio::ip::tcp;
    NetworkClient(tcp::socket* socket, u32 identity = 0) : BaseSocket(socket), _status(ConnectionStatus::AUTH_NONE), _identity(identity) { }

    void Listen();
    bool Connect(tcp::endpoint endpoint);
    bool Connect(u32 address, u16 port);
    bool Connect(std::string address, u16 port);

    ConnectionStatus GetStatus() { return _status; }
    void SetStatus(ConnectionStatus status) { _status = status; }

    u64 GetEntityId() { return _identity; }
    entt::entity GetEntity() { return static_cast<entt::entity>(_identity); }
    void SetEntityId(u64 identity) { _identity = identity; }
private:
    ConnectionStatus _status;
    u64 _identity;
};