#include "Sha256.h"
#include <stdarg.h>

void Sha256::Init()
{
	SHA256_Init(&_state);
}

void Sha256::Update(const u8* data, size_t size)
{
    SHA256_Update(&_state, data, size);
}

void Sha256::Update(const std::string& string)
{
    Update((u8 const*)string.c_str(), string.length());
}

void Sha256::Final(u8* dest)
{
    if (!dest)
        dest = _data;

    SHA256_Final(dest, &_state);
}

void Sha256::Hash(const u8* data, size_t length, u8* dest /* = nullptr */)
{
    Init();
    Update(data, length);
    Final(dest);
}

void Sha256::Hash(const std::string& string, u8* dest /* = nullptr */)
{
    Init();
    Update((u8 const*)string.c_str(), string.length());
    Final(dest);
}