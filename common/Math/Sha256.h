#pragma once
#include "../NovusTypes.h"

#include <string>
#include <openssl/sha.h>

class BigNumber;
class Sha256
{
public:
    void Init();
    void Update(const void* data, size_t size);
    void Update(const std::string& string);
    void Final(u8* dest = nullptr);

    // Hash functions as Init -> Update -> Final (In case we just need to fire update once)
    void Hash(const u8* data, size_t length, u8* dest = nullptr);
    void Hash(const std::string& string, u8* dest = nullptr);

    u8* GetData() { return _data; }
private:
    SHA256_CTX _state;
    u8 _data[SHA256_DIGEST_LENGTH];
};