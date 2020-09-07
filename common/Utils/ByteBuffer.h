#pragma once
#include "../NovusTypes.h"
#include "../Utils/SharedPool.h"
#include <entity/fwd.hpp>
#include <cassert>
#include <cstring>

class Bytebuffer
{
public:
    Bytebuffer(u8* inData = nullptr, size_t inSize = 128)
    {
        if (inData == nullptr)
        {
            _data = new u8[inSize];
            _hasOwnership = true;
        }
        else
        {
            _data = inData;
        }

        size = inSize;
    }
    ~Bytebuffer()
    {
        if (_hasOwnership)
        {
            delete[] _data;
            _data = nullptr;
        }
    }

    template <typename T>
    inline bool Get(T& val)
    {
        assert(_data != nullptr);

        size_t readSize = sizeof(T);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<T const*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    template <typename T>
    inline bool Get(T& val, size_t offset)
    {
        assert(_data != nullptr);

        size_t readSize = sizeof(T);
        if (!CanPerformRead(readSize, offset))
            return false;

        val = *reinterpret_cast<T const*>(&_data[offset]);
        return true;
    }
    inline bool GetI8(i8& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(i8);
        if (!CanPerformRead(readSize))
            return false;

        val = _data[readData];
        readData += readSize;
        return true;
    }
    inline bool GetU8(u8& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(u8);
        if (!CanPerformRead(readSize))
            return false;

        val = _data[readData];
        readData += readSize;
        return true;
    }
    inline bool GetBytes(u8* dest, size_t size)
    {
        assert(_data != nullptr);

        if (size == 0)
            return true;

        if (!CanPerformRead(size))
            return false;

        std::memcpy(dest, &_data[readData], size);
        readData += size;
        return true;
    }
    inline bool GetBytes(u8* dest, size_t size, size_t offset)
    {
        assert(_data != nullptr);

        if (size == 0)
            return true;

        if (!CanPerformRead(size, offset))
            return false;

        std::memcpy(dest, &_data[offset], size);
        return true;
    }
    inline bool GetI16(i16& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(i16);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<i16*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    inline bool GetU16(u16& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(u16);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<u16*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    inline bool GetI32(i32& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(i32);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<i32*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    inline bool GetU32(u32& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(u32);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<u32*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    inline bool GetF32(f32& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(f32);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<f32*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    inline bool GetI64(i64& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(i64);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<i64*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    inline bool GetU64(u64& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(u64);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<u64*>(&_data[readData]);
        readData += readSize;
        return true;
    }
    inline bool GetF64(f64& val)
    {
        assert(_data != nullptr);

        const size_t readSize = sizeof(f64);
        if (!CanPerformRead(readSize))
            return false;

        val = *reinterpret_cast<f64*>(_data[readData]);
        readData += readSize;
        return true;
    }
    inline void GetString(std::string& val)
    {
        assert(_data != nullptr);
        val.clear();
        while (readData < size)
        {
            char c = _data[readData++];
            if (c == 0)
                break;

            val += c;
        }
    }
    inline void GetString(std::string& val, i32 size)
    {
        assert(_data != nullptr);
        val.clear();

        for (i32 i = 0; i < size; i++)
        {
            val += _data[readData++];
        }
    }
    inline void GetStringByOffset(std::string& val, size_t offset)
    {
        assert(_data != nullptr);
        val.clear();
        while (offset < size)
        {
            char c = _data[offset++];
            if (c == 0)
                break;

            val += c;
        }
    }
    inline void GetStringByOffset(std::string& val, i32 size, size_t offset)
    {
        assert(_data != nullptr);
        val.clear();

        for (i32 i = 0; i < size; i++)
        {
            val += _data[offset++];
        }
    }

    template <typename T>
    inline bool Put(T val)
    {
        assert(_data != nullptr);

        size_t writeSize = sizeof(T);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], &val, writeSize);
        writtenData += writeSize;
        return true;
    }
    template <typename T>
    inline bool Put(T val, size_t offset)
    {
        assert(_data != nullptr);

        size_t writeSize = sizeof(T);
        if (!CanPerformWrite(writeSize, offset))
            return false;

        std::memcpy(&_data[offset], &val, writeSize);
        return true;
    }
    inline bool PutI8(i8 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i8);
        if (!CanPerformWrite(writeSize))
            return false;

        _data[writtenData] = val;
        writtenData += writeSize;
        return true;
    }
    inline bool PutU8(u8 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u8);
        if (!CanPerformWrite(writeSize))
            return false;

        _data[writtenData] = val;
        writtenData += writeSize;
        return true;
    }
    inline bool PutBytes(const u8* val, size_t size)
    {
        assert(_data != nullptr);

        if (!CanPerformWrite(size))
            return false;

        std::memcpy(&_data[writtenData], val, size);
        writtenData += size;
        return true;
    }
    inline bool PutI16(i16 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i16);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutU16(u16 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u16);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutI32(i32 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i32);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutU32(u32 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u32);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutF32(f32 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(f32);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutI64(i64 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(i64);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutU64(u64 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(u64);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutF64(f64 val)
    {
        assert(_data != nullptr);

        const size_t writeSize = sizeof(f64);
        if (!CanPerformWrite(writeSize))
            return false;

        std::memcpy(&_data[writtenData], reinterpret_cast<const u8*>(&val), writeSize);
        writtenData += writeSize;
        return true;
    }
    inline bool PutEnttId(entt::entity entity)
    {
        if constexpr (sizeof(entt::entity) == 4)
        {
            return PutU32(static_cast<u32>(entity));
        }
        else if constexpr (sizeof(entt::entity) == 4)
        {
            return PutU64(static_cast<u64>(entity));
        }
        else
        {
            assert(false);
            return false;
        }
    }
    inline bool PutEnttId(u32 val)
    {
        if constexpr (sizeof(entt::entity) == 4)
        {
            return PutU32(val);
        }
        else if constexpr (sizeof(entt::entity) == 4)
        {
            return PutU64(static_cast<u64>(val));
        }
        else
        {
            assert(false);
            return false;
        }
    }
    inline bool PutEnttId(u64 val)
    {
        if constexpr (sizeof(entt::entity) == 4)
        {
            return PutU32(static_cast<u32>(val));
        }
        else if constexpr (sizeof(entt::entity) == 4)
        {
            return PutU64(val);
        }
        else
        {
            assert(false);
            return false;
        }
    }
    inline size_t PutString(const std::string_view val)
    {
        assert(_data != nullptr);

        size_t writeSize = val.length();
        size_t writeSizeTotal = writeSize + 1;
        if (!CanPerformWrite(writeSizeTotal))
            return 0;

        std::memcpy(&_data[writtenData], val.data(), writeSize);
        writtenData += writeSize;
        _data[writtenData++] = 0;
        return writeSizeTotal;
    }

    inline bool CanPerformRead(size_t inSize)
    {
        return readData + inSize <= size;
    }
    inline bool CanPerformRead(size_t inSize, size_t offset)
    {
        return offset + inSize <= size;
    }
    inline bool CanPerformWrite(size_t inSize)
    {
        return writtenData + inSize <= size;
    }
    inline bool CanPerformWrite(size_t inSize, size_t offset)
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

    template <size_t size>
    static std::shared_ptr<Bytebuffer> Borrow()
    {
        static_assert(size <= 209715200);

        if constexpr (size <= 128)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer128.acquireOrCreate(nullptr, 128);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 512)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer512.acquireOrCreate(nullptr, 512);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 1024)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer1024.acquireOrCreate(nullptr, 1024);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 4096)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer4096.acquireOrCreate(nullptr, 4096);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 8192)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer8192.acquireOrCreate(nullptr, 8192);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 16384)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer16384.acquireOrCreate(nullptr, 16384);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 32768)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer32768.acquireOrCreate(nullptr, 32768);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 65536)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer65536.acquireOrCreate(nullptr, 65536);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 131072)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer131072.acquireOrCreate(nullptr, 131072);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 262144)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer262144.acquireOrCreate(nullptr, 262144);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 524288)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer524288.acquireOrCreate(nullptr, 524288);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 1048576)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer1048576.acquireOrCreate(nullptr, 1048576);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 8388608)
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer8388608.acquireOrCreate(nullptr, 8388608);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 209715200) // This is used for the Data Extractor, largest observed file in WOTLK is 65MB, however in BFA this has been observed to be 150MB
        {
            std::shared_ptr<Bytebuffer> buffer = _byteBuffer209715200.acquireOrCreate(nullptr, 209715200);
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
    }

private:
    u8* _data = nullptr;
    bool _hasOwnership = false;

    static SharedPool<Bytebuffer> _byteBuffer128;
    static SharedPool<Bytebuffer> _byteBuffer512;
    static SharedPool<Bytebuffer> _byteBuffer1024;
    static SharedPool<Bytebuffer> _byteBuffer4096;
    static SharedPool<Bytebuffer> _byteBuffer8192;
    static SharedPool<Bytebuffer> _byteBuffer16384;
    static SharedPool<Bytebuffer> _byteBuffer32768;
    static SharedPool<Bytebuffer> _byteBuffer65536;
    static SharedPool<Bytebuffer> _byteBuffer131072;
    static SharedPool<Bytebuffer> _byteBuffer262144;
    static SharedPool<Bytebuffer> _byteBuffer524288;
    static SharedPool<Bytebuffer> _byteBuffer1048576;
    static SharedPool<Bytebuffer> _byteBuffer8388608;
    static SharedPool<Bytebuffer> _byteBuffer209715200;
};
