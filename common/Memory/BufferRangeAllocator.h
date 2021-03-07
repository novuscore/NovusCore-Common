#pragma once
#include <NovusTypes.h>
#include <vector>

struct BufferRangeFrame
{
    size_t offset = 0;
    size_t size = 0;
};

class BufferRangeAllocator
{
public:
    BufferRangeAllocator() { }

    bool Init(size_t bufferOffset, size_t bufferSize);
    void Reset();

    bool Allocate(size_t size, BufferRangeFrame& frame);
    bool Free(const BufferRangeFrame& frame);
    bool Grow(size_t newSize);

    size_t Offset() { return _currentOffset; }
    size_t Size() { return _currentSize; }
    
private:
    size_t _currentOffset = 0;
    size_t _currentSize = 0;
    std::vector<BufferRangeFrame> _freeFrames;
};