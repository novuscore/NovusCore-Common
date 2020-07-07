#pragma once
#include "../NovusTypes.h"
#include <robin_hood.h>

class DataStorage
{
public:
    DataStorage() 
    {
        // Preallocate memory for 32 of each type
        _dataMapU8s.reserve(32);
        _dataMapU16s.reserve(32);
        _dataMapU32s.reserve(32);
        _dataMapU64s.reserve(32);
        _dataMapF32s.reserve(32);
        _dataMapF64s.reserve(32);
        _dataMapStrings.reserve(32);
        _dataMapPointers.reserve(32);
    }

    void Clear()
    {
        _dataMapU8s.clear();
        _dataMapU16s.clear();
        _dataMapU32s.clear();
        _dataMapU64s.clear();
        _dataMapF32s.clear();
        _dataMapF64s.clear();
        _dataMapStrings.clear();
        _dataMapPointers.clear();
    }

    bool PutU8(u32 nameHash, u8 val)
    {
        if (_dataMapU8s.find(nameHash) != _dataMapU8s.end())
            return false;

        _dataMapU8s[nameHash] = val;
        return true;
    }
    bool GetU8(u32 nameHash, u8& val)
    {
        if (_dataMapU8s.find(nameHash) == _dataMapU8s.end())
            return false;

        val = _dataMapU8s[nameHash];
        return true;
    }
    bool ClearU8(u32 nameHash)
    {
        if (_dataMapU8s.find(nameHash) == _dataMapU8s.end())
            return false;

        _dataMapU8s.erase(nameHash);
        return true;
    }

    bool PutU16(u16 nameHash, u16 val)
    {
        if (_dataMapU16s.find(nameHash) != _dataMapU16s.end())
            return false;

        _dataMapU16s[nameHash] = val;
        return true;
    }
    bool GetU16(u16 nameHash, u16& val)
    {
        if (_dataMapU16s.find(nameHash) == _dataMapU16s.end())
            return false;

        val = _dataMapU16s[nameHash];
        return true;
    }
    bool ClearU16(u32 nameHash)
    {
        if (_dataMapU16s.find(nameHash) == _dataMapU16s.end())
            return false;

        _dataMapU16s.erase(nameHash);
        return true;
    }

    bool PutU32(u32 nameHash, u32 val)
    {
        if (_dataMapU32s.find(nameHash) != _dataMapU32s.end())
            return false;

        _dataMapU32s[nameHash] = val;
        return true;
    }
    bool GetU32(u32 nameHash, u32& val)
    {
        if (_dataMapU32s.find(nameHash) == _dataMapU32s.end())
            return false;

        val = _dataMapU32s[nameHash];
        return true;
    }
    bool ClearU32(u32 nameHash)
    {
        if (_dataMapU32s.find(nameHash) == _dataMapU32s.end())
            return false;

        _dataMapU32s.erase(nameHash);
        return true;
    }

    bool PutU64(u32 nameHash, u64 val)
    {
        if (_dataMapU64s.find(nameHash) != _dataMapU64s.end())
            return false;

        _dataMapU64s[nameHash] = val;
        return true;
    }
    bool GetU64(u32 nameHash, u64& val)
    {
        if (_dataMapU64s.find(nameHash) == _dataMapU64s.end())
            return false;

        val = _dataMapU64s[nameHash];
        return true;
    }
    bool ClearU64(u32 nameHash)
    {
        if (_dataMapU64s.find(nameHash) == _dataMapU64s.end())
            return false;

        _dataMapU64s.erase(nameHash);
        return true;
    }

    bool PutF32(u32 nameHash, f32 val)
    {
        if (_dataMapF32s.find(nameHash) != _dataMapF32s.end())
            return false;

        _dataMapF32s[nameHash] = val;
        return true;
    }
    bool GetF32(u32 nameHash, f32& val)
    {
        if (_dataMapF32s.find(nameHash) == _dataMapF32s.end())
            return false;

        val = _dataMapF32s[nameHash];
        return true;
    }
    bool ClearF32(u32 nameHash)
    {
        if (_dataMapF32s.find(nameHash) == _dataMapF32s.end())
            return false;

        _dataMapF32s.erase(nameHash);
        return true;
    }

    bool PutF64(u32 nameHash, f64 val)
    {
        if (_dataMapF64s.find(nameHash) != _dataMapF64s.end())
            return false;

        _dataMapF64s[nameHash] = val;
        return true;
    }
    bool GetF64(u32 nameHash, f64& val)
    {
        if (_dataMapF64s.find(nameHash) == _dataMapF64s.end())
            return false;

        val = _dataMapF64s[nameHash];
        return true;
    }
    bool ClearF64(u32 nameHash)
    {
        if (_dataMapF64s.find(nameHash) == _dataMapF64s.end())
            return false;

        _dataMapF64s.erase(nameHash);
        return true;
    }

    bool PutString(u32 nameHash, std::string val)
    {
        if (_dataMapStrings.find(nameHash) != _dataMapStrings.end())
            return false;

        _dataMapStrings[nameHash] = val;
        return true;
    }
    bool GetString(u32 nameHash, std::string& val)
    {
        if (_dataMapStrings.find(nameHash) == _dataMapStrings.end())
            return false;

        val = _dataMapStrings[nameHash];
        return true;
    }
    bool ClearString(u32 nameHash)
    {
        if (_dataMapStrings.find(nameHash) == _dataMapStrings.end())
            return false;

        _dataMapStrings.erase(nameHash);
        return true;
    }

    bool PutPointer(u32 nameHash, void* val)
    {
        if (_dataMapPointers.find(nameHash) != _dataMapPointers.end())
            return false;

        _dataMapPointers[nameHash] = val;
        return true;
    }
    bool GetPointer(u32 nameHash, void*& val)
    {
        if (_dataMapPointers.find(nameHash) == _dataMapPointers.end())
            return false;

        val = _dataMapPointers[nameHash];
        return true;
    }
    bool ClearPointer(u32 nameHash)
    {
        if (_dataMapPointers.find(nameHash) == _dataMapPointers.end())
            return false;

        _dataMapPointers.erase(nameHash);
        return true;
    }

private:
    robin_hood::unordered_map<u32, u8> _dataMapU8s;
    robin_hood::unordered_map<u32, u16> _dataMapU16s;
    robin_hood::unordered_map<u32, u32> _dataMapU32s;
    robin_hood::unordered_map<u32, u64> _dataMapU64s;
    robin_hood::unordered_map<u32, f32> _dataMapF32s;
    robin_hood::unordered_map<u32, f64> _dataMapF64s;
    robin_hood::unordered_map<u32, std::string> _dataMapStrings;
    robin_hood::unordered_map<u32, void*> _dataMapPointers;
};