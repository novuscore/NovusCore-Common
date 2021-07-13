#include "BufferRangeAllocator.h"
#include <Utils/DebugHandler.h>

bool BufferRangeAllocator::Init(size_t bufferOffset, size_t bufferSize)
{
    if (_freeFrames.size() > 0)
        return false;

    _freeFrames.reserve(64);

    BufferRangeFrame& frame = _freeFrames.emplace_back();
    frame.offset = bufferOffset;
    frame.size = bufferSize;

    _currentOffset = bufferOffset;
    _currentSize = bufferSize;

    return true;
}

void BufferRangeAllocator::Reset()
{
    _freeFrames.clear();

    BufferRangeFrame& frame = _freeFrames.emplace_back();
    frame.offset = _currentOffset;
    frame.size = _currentSize;
}

bool BufferRangeAllocator::Allocate(size_t size, BufferRangeFrame& frame)
{
    if (size == 0)
        return false;

    size_t index = std::numeric_limits<size_t>().max();
    //size_t spaceLeft = std::numeric_limits<size_t>().max();

    for (size_t i = 0; i < _freeFrames.size(); i++)
    {
        BufferRangeFrame& freeFrame = _freeFrames[i];

        if (size < freeFrame.size)
        {
            index = i;
            break;

            // Best Fit (Use later when we know implementation works
            /*size_t space = freeFrame.size - size;
            if (space < spaceLeft)
            {
                spaceLeft = space;
                index = i;
            }*/
        }
    }

    if (index != std::numeric_limits<size_t>().max())
    {
        BufferRangeFrame& freeFrame = _freeFrames[index];

        frame.offset = freeFrame.offset;
        frame.size = size;

        freeFrame.offset += size;
        freeFrame.size -= size;

        if (freeFrame.size == 0)
            _freeFrames.erase(_freeFrames.begin() + index);

        return true;
    }

    return false;
}

bool BufferRangeAllocator::Allocate(size_t size, size_t alignment, BufferRangeFrame& frame)
{
    if (size == 0)
        return false;

    size_t index = std::numeric_limits<size_t>().max();
    //size_t spaceLeft = std::numeric_limits<size_t>().max();

    for (size_t i = 0; i < _freeFrames.size(); i++)
    {
        BufferRangeFrame& freeFrame = _freeFrames[i];

        size_t alignmentOffset = (alignment - (freeFrame.offset % alignment)) % alignment;
        size_t alignedSize = size + alignmentOffset;

        if (alignedSize < freeFrame.size)
        {
            index = i;
            break;

            // Best Fit (Use later when we know implementation works
            /*size_t space = freeFrame.size - size;
            if (space < spaceLeft)
            {
                spaceLeft = space;
                index = i;
            }*/
        }
    }

    if (index != std::numeric_limits<size_t>().max())
    {
        BufferRangeFrame& freeFrame = _freeFrames[index];

        size_t alignmentOffset = (alignment - (freeFrame.offset % alignment)) % alignment;
        size_t alignedSize = size + alignmentOffset;

        frame.offset = freeFrame.offset + alignmentOffset;
        frame.size = size;

        freeFrame.offset += alignedSize;
        freeFrame.size -= alignedSize;

        if (freeFrame.size == 0)
            _freeFrames.erase(_freeFrames.begin() + index);

        if (frame.offset % alignment != 0)
        {
            DebugHandler::PrintFatal("Did not allocate aligned");
        }

        return true;
    }

    return false;
}

bool BufferRangeAllocator::Free(const BufferRangeFrame& frame)
{
    BufferRangeFrame& newFreeFrame = _freeFrames.emplace_back();
    newFreeFrame = frame;

    // Validate if 'frame' overlaps with any free frame

    // Check if we can merge 'frame' with any free frame

    // Reclaim alignment as well

    return true;
}

bool BufferRangeAllocator::Grow(size_t newSize)
{
    size_t oldSize = _currentSize;
    if (newSize < oldSize)
        return false;

    size_t deltaSize = newSize - oldSize;

    BufferRangeFrame& frame = _freeFrames.emplace_back();
    frame.size = deltaSize;
    frame.offset = oldSize;

    _currentSize = newSize;

    // Check if we can merge 'frame' with any free frame
    return true;
}
