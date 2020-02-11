#pragma once
#include "../NovusTypes.h"
#include <openssl/sha.h>

class BigNumber;
class Sha256
{
public:
    void Init();
    void Update(const u8* data, size_t size);
    void Update(const std::string& string);
    void Update(BigNumber* bigNumber);
    void Final();

    // Hash functions as Init -> Update -> Final (In case we just need to fire update once)
    void Hash(const u8* data, size_t length);
    void Hash(const std::string& string);
    void Hash(size_t size, ...);

    u8* GetData() { return _data; }
    static const u32 SHA256_BLOCK_SIZE = 64;
private:
    SHA256_CTX _state;
    u8 _data[SHA256_DIGEST_LENGTH];
};