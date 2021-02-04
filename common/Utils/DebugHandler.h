/*
# MIT License

# Copyright(c) 2018-2019 NovusCore

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/
#pragma once

#include <string>
#include "../NovusTypes.h"

enum ColorCode
{
    GREEN = 10,
    YELLOW = 14,
    MAGENTA = 5,
    RED = 12,
};

#if defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
struct IDebugHandlerData {};

class DebugHandler
{
public:
    template <typename... Args>
    inline static void Print(std::string message, Args... args)
    {
        if (!_isInitialized)
        {
            Initialize();
        }

        printf((message + "\n").c_str(), args...);
    }

    template <typename... Args>
    inline static void PrintWarning(std::string message, Args... args)
    {
        if (!_isInitialized)
        {
            Initialize();
        }

        PrintColor("[Warning]: ", ColorCode::YELLOW);
        Print(message, args...);
    }

    template <typename... Args>
    inline static void PrintDeprecated(std::string message, Args... args)
    {
        if (!_isInitialized)
        {
            Initialize();
        }

        PrintColor("[Deprecated]: ", ColorCode::YELLOW);
        Print(message, args...);
    }

    template <typename... Args>
    inline static void PrintError(std::string message, Args... args)
    {
        if (!_isInitialized)
        {
            Initialize();
        }

        PrintColor("[Error]: ", ColorCode::MAGENTA);
        Print(message, args...);
    }

    template <typename... Args>
    inline static void PrintFatal(std::string message, Args... args)
    {
        if (!_isInitialized)
        {
            Initialize();
        }

        PrintColor("[Fatal]: ", ColorCode::RED);
        Print(message, args...);
        assert(false);
    }

    template <typename... Args>
    inline static void PrintSuccess(std::string message, Args... args)
    {
        if (!_isInitialized)
        {
            Initialize();
        }

        PrintColor("[Success]: ", ColorCode::GREEN);
        Print(message, args...);
    }

private:
    static void Initialize();

    template <typename... Args>
    inline static void PrintColor(std::string message, ColorCode color, Args... args)
    {
        PushColor(color);
        printf(message.c_str(), args...);
        PopColor();
    }

    static void PushColor(ColorCode color);
    static void PopColor();

private:
    static bool _isInitialized;
    static IDebugHandlerData* _data;
};
#if defined(__clang__)
#pragma GCC diagnostic pop
#endif
