/*
    https://github.com/mtrebi/memory-allocators/
    MIT License

    Copyright (c) 2016 Mariano Trebino

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
    */
#include "StackAllocator.h"
#include <stdlib.h>
#include <algorithm>
#ifdef _DEBUG
#include <iostream>
#endif
#include <cassert>

namespace Memory
{
    StackAllocator::StackAllocator(const std::size_t totalSize, std::string name, bool debug)
        : Allocator(totalSize, name, debug) 
    {

    }

    void StackAllocator::Init() 
    {
        assert(!_initialized); // We already initialized this allocator!
        if (_startPtr != nullptr) {
            free(_startPtr);
        }
        _startPtr = malloc(_totalSize);
        _offset = 0;

        _initialized = true;
    }

    StackAllocator::~StackAllocator() 
    {
        free(_startPtr);
        _startPtr = nullptr;
    }

    const std::size_t CalculatePadding(const std::size_t baseAddress, const std::size_t alignment) 
    {
        const std::size_t multiplier = (baseAddress / alignment) + 1;
        const std::size_t alignedAddress = multiplier * alignment;
        const std::size_t padding = alignedAddress - baseAddress;
        return padding;
    }

    const std::size_t CalculatePaddingWithHeader(const std::size_t baseAddress, const std::size_t alignment, const std::size_t headerSize) 
    {
        std::size_t padding = CalculatePadding(baseAddress, alignment);
        std::size_t neededSpace = headerSize;

        if (padding < neededSpace) {
            // Header does not fit - Calculate next aligned address that header fits
            neededSpace -= padding;

            // How many alignments I need to fit the header        
            if (neededSpace % alignment > 0) {
                padding += alignment * (1 + (neededSpace / alignment));
            }
            else {
                padding += alignment * (neededSpace / alignment);
            }
        }

        return padding;
    }

    void* StackAllocator::Allocate(const std::size_t size, const std::size_t alignment) 
    {
        assert(_initialized); // We need to initialize this allocator!

        const std::size_t currentAddress = reinterpret_cast<std::size_t>(_startPtr) + _offset;

        std::size_t padding = CalculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));

        if (_offset + padding + size > _totalSize) 
        {
            assert(false); // We overflowed our allocator

            return nullptr;
        }
        _offset += padding;

        const std::size_t nextAddress = currentAddress + padding;
        const std::size_t headerAddress = nextAddress - sizeof(AllocationHeader);
        AllocationHeader allocationHeader{ static_cast<char>(padding) };
        AllocationHeader* headerPtr = reinterpret_cast<AllocationHeader*>(headerAddress);
        headerPtr = &allocationHeader;

        _offset += size;

#ifdef _DEBUG
        if (_debug)
        {
            std::cout << _name << "\tAllocated " << "\t@C " << reinterpret_cast<void*>(currentAddress) << "\t@R " << reinterpret_cast<void*>(nextAddress) << "\tO " << _offset << "\tP " << padding << std::endl;
        }
#endif
        _used = _offset;
        _peak = std::max(_peak, _used);

        return (void*)(nextAddress);
    }

    void StackAllocator::Free(void* ptr) 
    {
        // Move offset back to clear address
        const std::size_t currentAddress = (std::size_t) ptr;
        const std::size_t headerAddress = currentAddress - sizeof(AllocationHeader);
        const AllocationHeader* allocationHeader{ (AllocationHeader*)headerAddress };

        _offset = currentAddress - allocationHeader->padding - (std::size_t) _startPtr;
        _used = _offset;

#ifdef _DEBUG
        if (_debug)
        {
            std::cout << _name << "\tFreed " << "\t@C " << (void*)currentAddress << "\t@F " << (void*)((char*)_startPtr + _offset) << "\tO " << _offset << std::endl;
        }
#endif
    }

    void StackAllocator::Reset() 
    {
        _offset = 0;
        _used = 0;
        _peak = 0;
    }
}