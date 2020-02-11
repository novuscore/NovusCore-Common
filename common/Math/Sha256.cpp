#include "Sha256.h"
#include "BigNumber.h"
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

void Sha256::Update(BigNumber* bigNumber)
{
    Update(bigNumber->BN2BinArray().get(), bigNumber->GetBytes());
}

void Sha256::Final()
{
    SHA256_Final(_data, &_state);
}

void Sha256::Hash(const u8* data, size_t length)
{
    Init();
    Update(data, length);
    Final();
}

void Sha256::Hash(const std::string& string)
{
    Init();
    Update((u8 const*)string.c_str(), string.length());
    Final();
}

void Sha256::Hash(size_t size, ...)
{
    Init();
    {
        va_list v;
        BigNumber* tempBn = nullptr;

        va_start(v, size);
        for (size_t i = 0; i < size; i++)
        {
            tempBn = va_arg(v, BigNumber*);
            Update(tempBn);
        }

        va_end(v);
    }
    Final();
}