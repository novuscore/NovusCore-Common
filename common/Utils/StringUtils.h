#pragma once
#include "../NovusTypes.h"

namespace StringUtils
{
    // This function assumes src to be a zero terminated sanitized string with
    // an even number of [0-9a-f] characters, and target to be sufficiently large
    void HexStrToBytes(const char* src, u8* target);
    std::string BytesToHexStr(u8* data, size_t length);
  
    i32 HexCharToInt(char input);

    std::string GetLineFromCin();

    std::vector<std::string> SplitString(std::string string, char delim = ' ');
    std::string EscapeString(std::string const& string);
    std::string FormatThousandSeparator(i32 n);
    bool EndsWith(std::string const& fullString, std::string const& ending);

#ifdef _WINDOWS
    std::wstring StringToWString(const std::string& s);
    std::string WStringToString(const std::wstring& wstr);
#endif

    template <typename... Args>
    inline i32 FormatString(char* buffer, size_t bufferSize, char const* format, Args... args)
    {
        i32 length = -1;
    #ifdef _WIN32
        length = sprintf_s(buffer, bufferSize, format, args...);
    #else
        (void)bufferSize; // Get rid of warning
        length = sprintf(buffer, format, args...);
    #endif
        assert(length > -1);
        return length;
    }

    // FNV-1a 32bit hashing algorithm.
    constexpr u32 fnv1a_32(char const* s, std::size_t count)
    {
        return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }

    constexpr size_t const_strlen(const char* s)
    {
        size_t size = 0;
        while (s[size]) 
        { 
            size++; 
        };

        return size;
    }

    struct StringHash
    {
        u32 computedHash;

        constexpr StringHash(u32 hash) noexcept : computedHash(hash) {}

        constexpr StringHash(const char* s) noexcept: computedHash(0)
        {
            computedHash = fnv1a_32(s, const_strlen(s));
        }
        constexpr StringHash(const char* s, std::size_t count) noexcept : computedHash(0)
        {
            computedHash = fnv1a_32(s, count);
        }
        constexpr StringHash(std::string_view s) noexcept : computedHash(0)
        {
            computedHash = fnv1a_32(s.data(),s.size());
        }
        StringHash(const StringHash& other) = default;

        constexpr operator u32()noexcept { return computedHash; }
    };
} // namespace StringUtils

constexpr StringUtils::StringHash operator"" _h(char const* s, std::size_t count)
{
    return StringUtils::StringHash{ s,count };
}