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
#pragma once
#include "../NovusTypes.h"

namespace Memory
{
    class Allocator
    {
    public:
        Allocator(const std::size_t totalSize, std::string name = "", bool debug = false);
        virtual ~Allocator();

        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) = 0;
        virtual void Free(void* ptr) = 0;
        virtual void Init() = 0;

        template<typename T, typename... Args>
        static T* New(Allocator* allocator, Args&& ... args)
        {
            void* memory = allocator->Allocate(sizeof(T), 8);
            T* object = new(memory) T(std::forward<Args>(args)...);

            return object;
        }

        template<typename T, typename... Args>
        static T* NewArray(Allocator* allocator, size_t count, Args&& ... args)
        {
            void* memory = allocator->Allocate(sizeof(T) * count, 8);
            T* typedArray = static_cast<T*>(memory);
            
            return typedArray;
        }

    protected:
        std::size_t _totalSize;
        std::size_t _used;
        std::size_t _peak;

        bool _debug;
        bool _initialized;
        std::string _name;
    };
}