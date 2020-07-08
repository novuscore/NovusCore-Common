#pragma once
#include "../NovusTypes.h"
#include <robin_hood.h>
#include <entity/fwd.hpp>

class DataStorage
{
public:
    DataStorage() 
    {
        // Preallocate memory for each type
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

    inline bool PutU8(u32 nameHash, u8 val)
    {
        if (_dataMapU8s.find(nameHash) != _dataMapU8s.end())
            return false;

        _dataMapU8s[nameHash] = val;
        return true;
    }
    inline void EmplaceU8(u32 nameHash, u8 val)
    {
        _dataMapU8s[nameHash] = val;
    }
    inline bool GetU8(u32 nameHash, u8& val)
    {
        if (_dataMapU8s.find(nameHash) == _dataMapU8s.end())
            return false;

        val = _dataMapU8s[nameHash];
        return true;
    }
    inline bool HasU8(u32 nameHash)
    {
        return _dataMapU8s.find(nameHash) != _dataMapU8s.end();
    }
    inline bool ClearU8(u32 nameHash)
    {
        if (_dataMapU8s.find(nameHash) == _dataMapU8s.end())
            return false;

        _dataMapU8s.erase(nameHash);
        return true;
    }

    inline bool PutU16(u16 nameHash, u16 val)
    {
        if (_dataMapU16s.find(nameHash) != _dataMapU16s.end())
            return false;

        _dataMapU16s[nameHash] = val;
        return true;
    }
    inline void EmplaceU16(u32 nameHash, u16 val)
    {
        _dataMapU16s[nameHash] = val;
    }
    inline bool GetU16(u16 nameHash, u16& val)
    {
        if (_dataMapU16s.find(nameHash) == _dataMapU16s.end())
            return false;

        val = _dataMapU16s[nameHash];
        return true;
    }
    inline bool HasU16(u32 nameHash)
    {
        return _dataMapU16s.find(nameHash) != _dataMapU16s.end();
    }
    inline bool ClearU16(u32 nameHash)
    {
        if (_dataMapU16s.find(nameHash) == _dataMapU16s.end())
            return false;

        _dataMapU16s.erase(nameHash);
        return true;
    }

    inline bool PutU32(u32 nameHash, u32 val)
    {
        if (_dataMapU32s.find(nameHash) != _dataMapU32s.end())
            return false;

        _dataMapU32s[nameHash] = val;
        return true;
    }
    inline void EmplaceU32(u32 nameHash, u32 val)
    {
        _dataMapU32s[nameHash] = val;
    }
    inline bool GetU32(u32 nameHash, u32& val)
    {
        if (_dataMapU32s.find(nameHash) == _dataMapU32s.end())
            return false;

        val = _dataMapU32s[nameHash];
        return true;
    }
    inline bool HasU32(u32 nameHash)
    {
        return _dataMapU32s.find(nameHash) != _dataMapU32s.end();
    }
    inline bool ClearU32(u32 nameHash)
    {
        if (_dataMapU32s.find(nameHash) == _dataMapU32s.end())
            return false;

        _dataMapU32s.erase(nameHash);
        return true;
    }

    inline bool PutU64(u32 nameHash, u64 val)
    {
        if (_dataMapU64s.find(nameHash) != _dataMapU64s.end())
            return false;

        _dataMapU64s[nameHash] = val;
        return true;
    }
    inline void EmplaceU64(u32 nameHash, u64 val)
    {
        _dataMapU64s[nameHash] = val;
    }
    inline bool GetU64(u32 nameHash, u64& val)
    {
        if (_dataMapU64s.find(nameHash) == _dataMapU64s.end())
            return false;

        val = _dataMapU64s[nameHash];
        return true;
    }
    inline bool HasU64(u32 nameHash)
    {
        return _dataMapU64s.find(nameHash) != _dataMapU64s.end();
    }
    inline bool ClearU64(u32 nameHash)
    {
        if (_dataMapU64s.find(nameHash) == _dataMapU64s.end())
            return false;

        _dataMapU64s.erase(nameHash);
        return true;
    }

    inline bool PutF32(u32 nameHash, f32 val)
    {
        if (_dataMapF32s.find(nameHash) != _dataMapF32s.end())
            return false;

        _dataMapF32s[nameHash] = val;
        return true;
    }
    inline void EmplaceF32(u32 nameHash, f32 val)
    {
        _dataMapF32s[nameHash] = val;
    }
    inline bool GetF32(u32 nameHash, f32& val)
    {
        if (_dataMapF32s.find(nameHash) == _dataMapF32s.end())
            return false;

        val = _dataMapF32s[nameHash];
        return true;
    }
    inline bool HasF32(u32 nameHash)
    {
        return _dataMapF32s.find(nameHash) != _dataMapF32s.end();
    }
    inline bool ClearF32(u32 nameHash)
    {
        if (_dataMapF32s.find(nameHash) == _dataMapF32s.end())
            return false;

        _dataMapF32s.erase(nameHash);
        return true;
    }

    inline bool PutF64(u32 nameHash, f64 val)
    {
        if (_dataMapF64s.find(nameHash) != _dataMapF64s.end())
            return false;

        _dataMapF64s[nameHash] = val;
        return true;
    }
    inline void EmplaceF64(u32 nameHash, f64 val)
    {
        _dataMapF64s[nameHash] = val;
    }
    inline bool GetF64(u32 nameHash, f64& val)
    {
        if (_dataMapF64s.find(nameHash) == _dataMapF64s.end())
            return false;

        val = _dataMapF64s[nameHash];
        return true;
    }
    inline bool HasF64(u32 nameHash)
    {
        return _dataMapF64s.find(nameHash) != _dataMapF64s.end();
    }
    inline bool ClearF64(u32 nameHash)
    {
        if (_dataMapF64s.find(nameHash) == _dataMapF64s.end())
            return false;

        _dataMapF64s.erase(nameHash);
        return true;
    }

    inline bool PutString(u32 nameHash, std::string val)
    {
        if (_dataMapStrings.find(nameHash) != _dataMapStrings.end())
            return false;

        _dataMapStrings[nameHash] = val;
        return true;
    }
    inline void EmplaceString(u32 nameHash, std::string val)
    {
        _dataMapStrings[nameHash] = val;
    }
    inline bool GetString(u32 nameHash, std::string& val)
    {
        if (_dataMapStrings.find(nameHash) == _dataMapStrings.end())
            return false;

        val = _dataMapStrings[nameHash];
        return true;
    }
    inline bool HasString(u32 nameHash)
    {
        return _dataMapStrings.find(nameHash) != _dataMapStrings.end();
    }
    inline bool ClearString(u32 nameHash)
    {
        if (_dataMapStrings.find(nameHash) == _dataMapStrings.end())
            return false;

        _dataMapStrings.erase(nameHash);
        return true;
    }

    inline bool PutPointer(u32 nameHash, void* val)
    {
        if (_dataMapPointers.find(nameHash) != _dataMapPointers.end())
            return false;

        _dataMapPointers[nameHash] = val;
        return true;
    }
    inline void EmplacePointer(u32 nameHash, void* val)
    {
        _dataMapPointers[nameHash] = val;
    }
    inline bool GetPointer(u32 nameHash, void*& val)
    {
        if (_dataMapPointers.find(nameHash) == _dataMapPointers.end())
            return false;

        val = _dataMapPointers[nameHash];
        return true;
    }
    inline bool HasPointer(u32 nameHash)
    {
        return _dataMapPointers.find(nameHash) != _dataMapPointers.end();
    }
    inline bool ClearPointer(u32 nameHash)
    {
        if (_dataMapPointers.find(nameHash) == _dataMapPointers.end())
            return false;

        _dataMapPointers.erase(nameHash);
        return true;
    }

    inline bool PutEntity(u32 nameHash, entt::entity val)
    {
        return PutU32(nameHash, entt::to_integral(val));
    }
    inline void EmplaceEntity(u32 nameHash, entt::entity val)
    {
        EmplaceU32(nameHash, entt::to_integral(val));
    }
    inline bool GetEntity(u32 nameHash, entt::entity& val)
    {
        if (_dataMapU32s.find(nameHash) == _dataMapU32s.end())
            return false;

        val = entt::entity(_dataMapU32s[nameHash]);
        return true;
    }
    inline bool HasEntity(u32 nameHash)
    {
        return HasU32(nameHash);
    }
    inline bool ClearEntity(u32 nameHash)
    {
        return ClearU32(nameHash);
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