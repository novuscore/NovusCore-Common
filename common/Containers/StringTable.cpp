#include "StringTable.h"
#include "../Utils/StringUtils.h"
#include "../Utils/Bytebuffer.h"
#include "../Utils/DynamicBytebuffer.h"

u32 StringTable::AddString(const std::string& string)
{
    std::unique_lock lock(_mutex);

    // We need the hash of the string
    u32 stringHash = StringUtils::fnv1a_32(string.c_str(), string.length());
    
    // Check if the string already exists in this table, if so return that index
    u32 index = 0;
    if (TryFindHashedString(stringHash, index))
    {
        return index;
    }

    // Add the string to the table
    index = static_cast<u32>(_strings.size());

    _strings.push_back(string);
    _hashes.push_back(stringHash);

    return index;
}

const std::string& StringTable::GetString(u32 index)
{
    std::shared_lock lock(_mutex);

    assert(index < _strings.size());

    return _strings[index];
}

u32 StringTable::GetStringHash(u32 index)
{
    std::shared_lock lock(_mutex);

    assert(index < _hashes.size());

    return _hashes[index];
}

bool StringTable::Serialize(Bytebuffer* bytebuffer) const
{
    // Here we use result as an inverse value to calculate cheaply if we serialized successfully
    bool result = false;

    // First we need to calculate the total size of our strings
    u32 totalSize = 0;

    for (const auto& string : _strings)
    {
        totalSize += static_cast<u32>(string.size())+1;
    }

    // Then we push this value into the byteBuffer
    bytebuffer->Put<u32>(totalSize);

    // Then we go ahead and put each string
    for (const auto& string : _strings)
    {
        result |= !bytebuffer->PutBytes((u8*)(string.c_str()), string.size()+1);
    }

    return !result;
}

bool StringTable::Serialize(DynamicBytebuffer* bytebuffer) const
{
    // Here we use result as an inverse value to calculate cheaply if we serialized successfully
    bool result = false;

    // First we need to calculate the total size of our strings
    u32 totalSize = 0;

    for (const auto& string : _strings)
    {
        totalSize += static_cast<u32>(string.size())+1;
    }

    // Then we push this value into the byteBuffer
    if (!bytebuffer->Put<u32>(totalSize))
        return false;

    // Then we go ahead and put each string
    for (const auto& string : _strings)
    {
        result |= !bytebuffer->PutBytes((u8*)(string.c_str()), string.size()+1);
    }

    return !result;
}

bool StringTable::Deserialize(Bytebuffer* bytebuffer)
{
    // First we read the total size of strings to read
    u32 totalSize;
    if (!bytebuffer->GetU32(totalSize))
    {
        assert(false);
        return false;
    }

    if (totalSize > bytebuffer->GetReadSpace())
        return false;

    if (totalSize == 0)
        return true;

    std::string string;
    u32 readSize = 0;
    while(readSize < totalSize)
    {
        if (!bytebuffer->GetString(string))
            return false;

        u32 hashedString = StringUtils::fnv1a_32(string.c_str(), string.size());
        _strings.push_back(string);
        _hashes.push_back(hashedString);
        readSize += static_cast<u32>(string.length()+1);
    }

    return true;
}

bool StringTable::Deserialize(DynamicBytebuffer* bytebuffer)
{
    // First we read the total size of strings to read
    u32 totalSize;
    if (!bytebuffer->GetU32(totalSize))
    {
        assert(false);
        return false;
    }

    if (totalSize > bytebuffer->GetReadSpace())
        return false;

    if (totalSize == 0)
        return true;

    std::string string;
    u32 readSize = 0;
    while(readSize < totalSize)
    {
        if (!bytebuffer->GetString(string))
            return false;

        u32 hashedString = StringUtils::fnv1a_32(string.c_str(), string.size());
        _strings.push_back(string);
        _hashes.push_back(hashedString);
        readSize += static_cast<u32>(string.length()+1);
    }

    return true;
}

void StringTable::CopyFrom(StringTable& other)
{
    std::unique_lock ourLock(_mutex);
    std::shared_lock theirLock(other._mutex);

    _strings = other._strings;
    _hashes = other._hashes;
}

bool StringTable::TryFindHashedString(u32 hash, u32& index) const
{
    for (size_t i = 0; i < _hashes.size(); i++)
    {
        if (hash == _hashes[i])
        {
            index = static_cast<u32>(i);
            return true;
        }
    }

    return false;
}

void StringTable::Clear()
{
    std::unique_lock ourLock(_mutex);

    _strings.clear();
    _hashes.clear();
}