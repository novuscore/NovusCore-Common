#pragma once
#include "../NovusTypes.h"
#include <robin_hood.h>
#include <entity/entity.hpp>

class DataStorage
{
public:
    DataStorage() 
    {
        // Preallocate memory for 32 of each type
        const u32 PREALLOCATE_AMOUNT = 32;
        _dataMapU8s.reserve(PREALLOCATE_AMOUNT);
        _dataMapU16s.reserve(PREALLOCATE_AMOUNT);
        _dataMapU32s.reserve(PREALLOCATE_AMOUNT);
        _dataMapU64s.reserve(PREALLOCATE_AMOUNT);
        _dataMapF32s.reserve(PREALLOCATE_AMOUNT);
        _dataMapF64s.reserve(PREALLOCATE_AMOUNT);
        _dataMapStrings.reserve(PREALLOCATE_AMOUNT);
        _dataMapPointers.reserve(PREALLOCATE_AMOUNT);
        _dataMapEntities.reserve(PREALLOCATE_AMOUNT);
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
        _dataMapEntities.clear();
    }

    bool PutU8(u32 nameHash, u8 val)
    {
        if (_dataMapU8s.find(nameHash) != _dataMapU8s.end())
            return false;

        _dataMapU8s[nameHash] = val;
        return true;
    }
    void EmplaceU8(u32 nameHash, u8 val)
    {
        _dataMapU8s[nameHash] = val;
    }
    bool GetU8(u32 nameHash, u8& val)
    {
        if (_dataMapU8s.find(nameHash) == _dataMapU8s.end())
            return false;

        val = _dataMapU8s[nameHash];
        return true;
    }
    bool HasU8(u32 nameHash)
    {
        return _dataMapU8s.find(nameHash) != _dataMapU8s.end();
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
    void EmplaceU16(u32 nameHash, u16 val)
    {
        _dataMapU16s[nameHash] = val;
    }
    bool GetU16(u16 nameHash, u16& val)
    {
        if (_dataMapU16s.find(nameHash) == _dataMapU16s.end())
            return false;

        val = _dataMapU16s[nameHash];
        return true;
    }
    bool HasU16(u32 nameHash)
    {
        return _dataMapU16s.find(nameHash) != _dataMapU16s.end();
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
    void EmplaceU32(u32 nameHash, u32 val)
    {
        _dataMapU32s[nameHash] = val;
    }
    bool GetU32(u32 nameHash, u32& val)
    {
        if (_dataMapU32s.find(nameHash) == _dataMapU32s.end())
            return false;

        val = _dataMapU32s[nameHash];
        return true;
    }
    bool HasU32(u32 nameHash)
    {
        return _dataMapU32s.find(nameHash) != _dataMapU32s.end();
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
    void EmplaceU64(u32 nameHash, u64 val)
    {
        _dataMapU64s[nameHash] = val;
    }
    bool GetU64(u32 nameHash, u64& val)
    {
        if (_dataMapU64s.find(nameHash) == _dataMapU64s.end())
            return false;

        val = _dataMapU64s[nameHash];
        return true;
    }
    bool HasU64(u32 nameHash)
    {
        return _dataMapU64s.find(nameHash) != _dataMapU64s.end();
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
    void EmplaceF32(u32 nameHash, f32 val)
    {
        _dataMapF32s[nameHash] = val;
    }
    bool GetF32(u32 nameHash, f32& val)
    {
        if (_dataMapF32s.find(nameHash) == _dataMapF32s.end())
            return false;

        val = _dataMapF32s[nameHash];
        return true;
    }
    bool HasF32(u32 nameHash)
    {
        return _dataMapF32s.find(nameHash) != _dataMapF32s.end();
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
    void EmplaceF64(u32 nameHash, f64 val)
    {
        _dataMapF64s[nameHash] = val;
    }
    bool GetF64(u32 nameHash, f64& val)
    {
        if (_dataMapF64s.find(nameHash) == _dataMapF64s.end())
            return false;

        val = _dataMapF64s[nameHash];
        return true;
    }
    bool HasF64(u32 nameHash)
    {
        return _dataMapF64s.find(nameHash) != _dataMapF64s.end();
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
    void EmplaceString(u32 nameHash, std::string val)
    {
        _dataMapStrings[nameHash] = val;
    }
    bool GetString(u32 nameHash, std::string& val)
    {
        if (_dataMapStrings.find(nameHash) == _dataMapStrings.end())
            return false;

        val = _dataMapStrings[nameHash];
        return true;
    }
    bool HasString(u32 nameHash)
    {
        return _dataMapStrings.find(nameHash) != _dataMapStrings.end();
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
    void EmplacePointer(u32 nameHash, void* val)
    {
        _dataMapPointers[nameHash] = val;
    }
    bool GetPointer(u32 nameHash, void*& val)
    {
        if (_dataMapPointers.find(nameHash) == _dataMapPointers.end())
            return false;

        val = _dataMapPointers[nameHash];
        return true;
    }
    bool HasPointer(u32 nameHash)
    {
        return _dataMapPointers.find(nameHash) != _dataMapPointers.end();
    }
    bool ClearPointer(u32 nameHash)
    {
        if (_dataMapPointers.find(nameHash) == _dataMapPointers.end())
            return false;

        _dataMapPointers.erase(nameHash);
        return true;
    }

    bool PutEntity(u32 nameHash, entt::entity val)
    {
        if (_dataMapEntities.find(nameHash) != _dataMapEntities.end())
            return false;

        _dataMapEntities[nameHash] = val;
        return true;
    }
    void EmplaceEntity(u32 nameHash, entt::entity val)
    {
        _dataMapEntities[nameHash] = val;
    }
    bool GetEntity(u32 nameHash, entt::entity& val)
    {
        if (_dataMapEntities.find(nameHash) == _dataMapEntities.end())
            return false;

        val = _dataMapEntities[nameHash];
        return true;
    }
    bool HasEntity(u32 nameHash)
    {
        return _dataMapEntities.find(nameHash) != _dataMapEntities.end();
    }
    bool ClearEntity(u32 nameHash)
    {
        if (_dataMapEntities.find(nameHash) == _dataMapEntities.end())
            return false;

        _dataMapEntities.erase(nameHash);
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
    robin_hood::unordered_map<u32, entt::entity> _dataMapEntities;
};