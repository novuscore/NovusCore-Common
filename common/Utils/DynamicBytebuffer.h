#pragma once
#include "../NovusTypes.h"
#include <cassert>
#include <cstring>

class DynamicBytebuffer
{
public:
    DynamicBytebuffer(size_t inSize)
    {
        _data = new u8[inSize];
        size = inSize;
    }
    ~DynamicBytebuffer()
    {
        delete[] _data;
        _data = nullptr;
    }

    template <typename T>
    inline bool Get(T& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<T const*>(&_data[readData]);
        readData += sizeof(T);
        return true;
    }
    template <typename T>
    inline bool Get(T& val, size_t offset)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<T const*>(&_data[offset]);
        return true;
    }
    inline bool GetI8(i8& val)
    {
        assert(_data != nullptr);

        val = _data[readData];
        readData += sizeof(i8);
        return true;
    }
    inline bool GetU8(u8& val)
    {
        assert(_data != nullptr);

        val = _data[readData];
        readData += sizeof(u8);
        return true;
    }
    inline bool GetBytes(u8* dest, size_t size)
    {
        assert(_data != nullptr);

        std::memcpy(dest, &_data[readData], size);
        readData += size;
        return true;
    }
    inline bool GetBytes(u8* dest, size_t size, size_t offset)
    {
        assert(_data != nullptr);

        std::memcpy(dest, &_data[offset], size);
        return true;
    }
    inline bool GetI16(i16& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<i16*>(&_data[readData]);
        readData += sizeof(i16);
        return true;
    }
    inline bool GetU16(u16& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<u16*>(&_data[readData]);
        readData += sizeof(u16);
        return true;
    }
    inline bool GetI32(i32& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<i32*>(&_data[readData]);
        readData += sizeof(i32);
        return true;
    }
    inline bool GetU32(u32& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<u32*>(&_data[readData]);
        readData += sizeof(u32);
        return true;
    }
    inline bool GetF32(f32& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<f32*>(&_data[readData]);
        readData += sizeof(f32);
        return true;
    }
    inline bool GetI64(i64& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<i64*>(&_data[readData]);
        readData += sizeof(i64);
        return true;
    }
    inline bool GetU64(u64& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<u64*>(&_data[readData]);
        readData += sizeof(u64);
        return true;
    }
    inline bool GetF64(f64& val)
    {
        assert(_data != nullptr);

        val = *reinterpret_cast<f64*>(_data[readData]);
        readData += sizeof(f64);
        return true;
    }
    inline bool GetString(char*& val)
    {
        assert(_data != nullptr);

        if (!CanPerformRead(1))
            return false;

        val = reinterpret_cast<char*>(&_data[readData]);
        while (readData < size)
        {
            char c = _data[readData++];
            if (c == 0)
                break;
        }

        return true;
    }
    inline bool GetString(std::string& val)
    {
        assert(_data != nullptr);

        if (!CanPerformRead(1))
            return false;

        val.clear();
        while (readData < size)
        {
            char c = _data[readData++];
            if (c == 0)
                break;

            val += c;
        }

        return true;
    }
    inline bool GetString(std::string& val, i32 size)
    {
        assert(_data != nullptr);

        if (!CanPerformRead(size))
            return false;

        val.clear();

        for (i32 i = 0; i < size; i++)
        {
            val += _data[readData++];
        }

        return true;
    }
    inline bool GetStringByOffset(std::string& val, size_t offset)
    {
        assert(_data != nullptr);

        if (!CanPerformRead(1, offset))
            return false;

        val.clear();
        while (offset < size)
        {
            char c = _data[offset++];
            if (c == 0)
                break;

            val += c;
        }

        return true;
    }
    inline bool GetStringByOffset(std::string& val, i32 size, size_t offset)
    {
        assert(_data != nullptr);

        if (!CanPerformRead(size, offset))
            return false;

        val.clear();

        for (i32 i = 0; i < size; i++)
        {
            val += _data[offset++];
        }

        return true;
    }

    template <typename T>
    inline bool Put(T val)
    {
        assert(_data != nullptr);

        size_t writeSize = sizeof(T);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], &val, writeSize);
        writtenData += writeSize;
        return true;
    }
    template <typename T>
    inline bool Put(T val, size_t offset)
    {
        assert(_data != nullptr);

        size_t writeSize = sizeof(T);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[offset], &val, writeSize);
        return true;
    }
    inline bool PutI8(i8 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i8);
        CheckStorageSpace(writeSize);

        _data[writtenData] = val;
        writtenData += writeSize;
        return true;
    }
    inline bool PutU8(u8 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u8);
        CheckStorageSpace(writeSize);

        _data[writtenData] = val;
        writtenData += writeSize;
        return true;
    }
    inline bool PutBytes(const u8* val, size_t size)
    {
        assert(_data != nullptr);

        CheckStorageSpace(size);

        std::memcpy(&_data[writtenData], val, size);
        writtenData += size;
        return true;
    }
    inline bool PutI16(i16 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i16);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutU16(u16 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u16);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutI32(i32 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i32);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutU32(u32 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u32);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutF32(f32 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(f32);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutI64(i64 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i64);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutU64(u64 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u64);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutF64(f64 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(f64);
        CheckStorageSpace(writeSize);

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline size_t PutString(const std::string_view val)
    {
        assert(_data != nullptr);

        size_t writeSize = val.length();
        size_t writeSizeTotal = writeSize + 1;
        CheckStorageSpace(writeSizeTotal);

        std::memcpy(&_data[writtenData], val.data(), writeSize);
        writtenData += writeSize;
        _data[writtenData++] = 0;
        return writeSizeTotal;
    }

    inline void CheckStorageSpace(size_t inSize)
    {
        // Reallocate if true
        if (writtenData + inSize > size)
        {
            u8* newArr = new u8[size * 2];

            if (writtenData > 0)
            {
                memcpy(newArr, _data, writtenData);
            }

            delete[] _data;
            _data = newArr;
        }
    }
    inline void SetSize(size_t inSize)
    {
        u8* newArr = new u8[size];

        if (writtenData > 0)
        {
            size_t bytesToCopy = glm::min(writtenData, size);

            memcpy(newArr, _data, bytesToCopy);
        }

        delete[] _data;
        _data = newArr;
    }    
    
    inline bool CanPerformRead(size_t inSize)
    {
        return readData + inSize <= size;
    }
    inline bool CanPerformRead(size_t inSize, size_t offset)
    {
        return offset + inSize <= size;
    }

    inline void SetOwnership(bool hasOwnership) { _hasOwnership = hasOwnership; }
    inline bool HasOwnership() { return _hasOwnership; }

    inline bool SkipRead(size_t bytes) 
    {
        if (readData + bytes > size)
            return false;

        readData += bytes;
        return true;
    }
    inline bool SkipWrite(size_t bytes) 
    {
        if (writtenData + bytes > size)
            return false;

        writtenData += bytes;
        return true;
    }
    inline void Reset()
    {
        writtenData = 0;
        readData = 0;
    }

    inline bool IsEmpty() { return writtenData == 0; }
    inline bool IsFull() { return writtenData == size; }
    inline size_t GetSpace() { return size - writtenData; }
    inline size_t GetReadSpace() { return size - readData; }
    inline size_t GetActiveSize() { return writtenData - readData; }

    size_t writtenData = 0;
    size_t readData = 0;
    size_t size = 0;

    u8* GetDataPointer() { return _data; }
    u8* GetReadPointer() { return _data + readData; }
    u8* GetWritePointer() { return _data + writtenData; }

private:
    u8* _data = nullptr;
    bool _hasOwnership = false;
};
