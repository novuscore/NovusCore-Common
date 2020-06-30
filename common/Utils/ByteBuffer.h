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
            if (_byteBuffer128.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 128);
                _byteBuffer128.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer128.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 512)
        {
            if (_byteBuffer512.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 512);
                _byteBuffer512.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer512.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 1024)
        {
            if (_byteBuffer1024.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 1024);
                _byteBuffer1024.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer1024.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 4096)
        {
            if (_byteBuffer4096.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 4096);
                _byteBuffer4096.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer4096.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 8192)
        {
            if (_byteBuffer8192.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 8192);
                _byteBuffer8192.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer8192.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 16384)
        {
            if (_byteBuffer16384.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 16384);
                _byteBuffer16384.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer16384.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 32768)
        {
            if (_byteBuffer32768.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 32768);
                _byteBuffer32768.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer32768.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 65536)
        {
            if (_byteBuffer65536.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 65536);
                _byteBuffer65536.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer65536.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 131072)
        {
            if (_byteBuffer131072.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 131072);
                _byteBuffer131072.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer131072.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 262144)
        {
            if (_byteBuffer262144.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 262144);
                _byteBuffer262144.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer262144.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 524288)
        {
            if (_byteBuffer524288.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 524288);
                _byteBuffer524288.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer524288.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 1048576)
        {
            if (_byteBuffer1048576.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 1048576);
                _byteBuffer1048576.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer1048576.acquire();
            buffer->size = size;
            buffer->Reset();

            return buffer;
        }
        else if constexpr (size <= 209715200) // This is used for the Data Extractor, largest observed file in WOTLK is 65MB, however in BFA this has been observed to be 150MB
        {
            if (_byteBuffer209715200.empty())
            {
                Bytebuffer* newDataStore = new Bytebuffer(nullptr, 209715200);
                _byteBuffer209715200.add(std::unique_ptr<Bytebuffer>(newDataStore));
            }

            std::shared_ptr<Bytebuffer> buffer = _byteBuffer209715200.acquire();
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
    static SharedPool<Bytebuffer> _byteBuffer209715200;
};
