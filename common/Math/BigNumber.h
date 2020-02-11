#pragma once
#include "../NovusTypes.h"
#include <memory>
#include <string>

struct bignum_st;
class BigNumber
{
public:
    BigNumber();
    BigNumber(BigNumber const& bn);
    BigNumber(u32);
    ~BigNumber();

    void SetUInt32(u32);
    void SetUInt64(u64);
    u32 GetUInt32();

    std::string BN2Hex() const;
    std::string BN2Dec() const;
    void Hex2BN(char const* string);
    void Bin2BN(u8 const* data, i32 size);
    std::unique_ptr<u8[]> BN2BinArray(size_t size = 0);

    void Rand(size_t bits);
    BigNumber Exponential(BigNumber const&);
    BigNumber ModExponential(BigNumber const& bigNum1, BigNumber const& bigNum2);

    bool IsZero() const;
    bool IsNegative() const;
    i32 GetBytes(void);

    BigNumber& operator=(BigNumber const& bigNum);
    BigNumber operator+=(BigNumber const& bigNum);
    BigNumber operator+(BigNumber const& bigNum)
    {
        BigNumber t(*this);
        return t += bigNum;
    }
    BigNumber operator-=(BigNumber const& bigNum);
    BigNumber operator-(BigNumber const& bigNum)
    {
        BigNumber t(*this);
        return t -= bigNum;
    }
    BigNumber operator*=(BigNumber const& bigNum);
    BigNumber operator*(BigNumber const& bigNum)
    {
        BigNumber t(*this);
        return t *= bigNum;
    }
    BigNumber operator/=(BigNumber const& bigNum);
    BigNumber operator/(BigNumber const& bigNum)
    {
        BigNumber t(*this);
        return t /= bigNum;
    }
    BigNumber operator%=(BigNumber const& bigNum);
    BigNumber operator%(BigNumber const& bigNum)
    {
        BigNumber t(*this);
        return t %= bigNum;
    }

    struct bignum_st* BigNum() { return _bigNum; }

private:
    struct bignum_st* _bigNum;
};