#pragma once
#include <vector>
#include <shared_mutex>
#include <atomic>

template <typename T>
class SafeVector
{
public:
    void ReadLock(const std::function<void(const std::vector<T>&)> callback) const
    {
        std::shared_lock lock(_mutex);
        callback(_vector);
    }

    void WriteLock(const std::function<void(std::vector<T>&)> callback)
    {
        std::unique_lock lock(_mutex);
        callback(_vector);
    }

    const T& ReadGet(size_t index) const
    {
        std::shared_lock lock(_mutex);
        return _vector[index];
    }

    size_t Size() const
    {
        std::shared_lock lock(_mutex);
        return _vector.size();
    }

    void Resize(size_t newSize)
    {
        std::unique_lock lock(_mutex);
        _vector.resize(newSize);
    }

    void Reserve(size_t reserveSize)
    {
        std::unique_lock lock(_mutex);
        _vector.reserve(reserveSize);
    }

    void Clear()
    {
        std::unique_lock lock(_mutex);
        _vector.clear();
    }

    T& EmplaceBack()
    {
        std::unique_lock lock(_mutex);
        return _vector.emplace_back();
    }

    void PushBack(T& obj)
    {
        std::unique_lock lock(_mutex);
        _vector.push_back(obj);
    }

private:
    mutable std::shared_mutex _mutex;
    std::vector<T> _vector;
};