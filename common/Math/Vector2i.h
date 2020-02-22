#pragma once
#include <string>
#include "Math.h"

class Vector2;
class Vector3;

// Right handed coordinate system, positive X points north and positive Y points west
class Vector2i
{
public:
    // Constructors
    Vector2i() {};
    Vector2i(float inX, float inY)
    {
        x = static_cast<int>(inX);
        y = static_cast<int>(inY);
    }
    Vector2i(float in)
    {
        x = static_cast<int>(in);
        y = static_cast<int>(in);
    }
    Vector2i(unsigned char inX, unsigned char inY)
    {
        x = static_cast<int>(inX);
        y = static_cast<int>(inY);
    }
    Vector2i(unsigned char in)
    {
        x = static_cast<int>(in);
        y = static_cast<int>(in);
    }
    Vector2i(unsigned short inX, unsigned short inY)
    {
        x = static_cast<int>(inX);
        y = static_cast<int>(inY);
    }
    Vector2i(unsigned short in)
    {
        x = static_cast<int>(in);
        y = static_cast<int>(in);
    }
    Vector2i(unsigned int inX, unsigned int inY)
    {
        x = static_cast<int>(inX);
        y = static_cast<int>(inY);
    }
    Vector2i(unsigned int in)
    {
        x = static_cast<int>(in);
        y = static_cast<int>(in);
    }
    Vector2i(signed char inX, signed char inY)
    {
        x = static_cast<int>(inX);
        y = static_cast<int>(inY);
    }
    Vector2i(signed char in)
    {
        x = static_cast<int>(in);
        y = static_cast<int>(in);
    }
    Vector2i(signed short inX, signed short inY)
    {
        x = static_cast<int>(inX);
        y = static_cast<int>(inY);
    }
    Vector2i(signed short in)
    {
        x = static_cast<int>(in);
        y = static_cast<int>(in);
    }
    Vector2i(signed int inX, signed int inY)
    {
        x = static_cast<int>(inX);
        y = static_cast<int>(inY);
    }
    Vector2i(signed int in)
    {
        x = static_cast<int>(in);
        y = static_cast<int>(in);
    }

    Vector2i(const Vector2i& in)
    {
        x = in.x;
        y = in.y;
    }
    Vector2i(const Vector2& in);
    Vector2i(const Vector3& in);

    // Returns a nicely formatted string of the vector
    inline std::string ToString()
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    // Overloaded operators
    inline Vector2i operator+(const Vector2i& other)
    {
        return Vector2i(x + other.x, y + other.y);
    }

    inline Vector2i operator+(const float other)
    {
        return Vector2i(x + other, y + other);
    }

    inline Vector2i operator+(const unsigned char other)
    {
        return Vector2i(x + other, y + other);
    }

    inline Vector2i operator+(const unsigned short other)
    {
        return Vector2i(x + other, y + other);
    }

    inline Vector2i operator+(const unsigned int other)
    {
        return Vector2i(x + other, y + other);
    }

    inline Vector2i operator+(const signed char other)
    {
        return Vector2i(x + other, y + other);
    }

    inline Vector2i operator+(const signed short other)
    {
        return Vector2i(x + other, y + other);
    }

    inline Vector2i operator+(const signed int other)
    {
        return Vector2i(x + other, y + other);
    }

    inline Vector2i operator-(const Vector2i& other)
    {
        return Vector2i(x - other.x, y - other.y);
    }

    inline Vector2i operator-(const float other)
    {
        return Vector2i(x - other, y - other);
    }

    inline Vector2i operator-(const unsigned char other)
    {
        return Vector2i(x - other, y - other);
    }

    inline Vector2i operator-(const unsigned short other)
    {
        return Vector2i(x - other, y - other);
    }

    inline Vector2i operator-(const unsigned int other)
    {
        return Vector2i(x - other, y - other);
    }

    inline Vector2i operator-(const signed char other)
    {
        return Vector2i(x - other, y - other);
    }

    inline Vector2i operator-(const signed short other)
    {
        return Vector2i(x - other, y - other);
    }

    inline Vector2i operator-(const signed int other)
    {
        return Vector2i(x - other, y - other);
    }

    inline Vector2i operator*(const Vector2i& other)
    {
        return Vector2i(x * other.x, y * other.y);
    }

    inline Vector2i operator*(const float other)
    {
        return Vector2i(x * other, y * other);
    }

    inline Vector2i operator*(const unsigned char other)
    {
        return Vector2i(x * other, y * other);
    }

    inline Vector2i operator*(const unsigned short other)
    {
        return Vector2i(x * other, y * other);
    }

    inline Vector2i operator*(const unsigned int other)
    {
        return Vector2i(x * other, y * other);
    }

    inline Vector2i operator*(const signed char other)
    {
        return Vector2i(x * other, y * other);
    }

    inline Vector2i operator*(const signed short other)
    {
        return Vector2i(x * other, y * other);
    }

    inline Vector2i operator*(const signed int other)
    {
        return Vector2i(x * other, y * other);
    }

    inline Vector2i operator/(const Vector2i& other)
    {
        return Vector2i(x / other.x, y / other.y);
    }

    inline Vector2i operator/(const float other)
    {
        return Vector2i(x / other, y / other);
    }

    inline Vector2i operator/(const unsigned char other)
    {
        return Vector2i(x / other, y / other);
    }

    inline Vector2i operator/(const unsigned short other)
    {
        return Vector2i(x / other, y / other);
    }

    inline Vector2i operator/(const unsigned int other)
    {
        return Vector2i(x / other, y / other);
    }

    inline Vector2i operator/(const signed char other)
    {
        return Vector2i(x / other, y / other);
    }

    inline Vector2i operator/(const signed short other)
    {
        return Vector2i(x / other, y / other);
    }

    inline Vector2i operator/(const signed int other)
    {
        return Vector2i(x / other, y / other);
    }

    // opAssign
    inline Vector2i& operator+=(const Vector2i& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline Vector2i& operator+=(const float other)
    {
        x += static_cast<int>(other);
        y += static_cast<int>(other);
        return *this;
    }

    inline Vector2i& operator+=(const unsigned char other)
    {
        x += other;
        y += other;
        return *this;
    }

    inline Vector2i& operator+=(const unsigned short other)
    {
        x += other;
        y += other;
        return *this;
    }

    inline Vector2i& operator+=(const unsigned int other)
    {
        x += other;
        y += other;
        return *this;
    }

    inline Vector2i& operator+=(const signed char other)
    {
        x += other;
        y += other;
        return *this;
    }

    inline Vector2i& operator+=(const signed short other)
    {
        x += other;
        y += other;
        return *this;
    }

    inline Vector2i& operator+=(const signed int other)
    {
        x += other;
        y += other;
        return *this;
    }

    inline Vector2i& operator-=(const Vector2i& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline Vector2i& operator-=(const float other)
    {
        x -= static_cast<int>(other);
        y -= static_cast<int>(other);
        return *this;
    }

    inline Vector2i& operator-=(const unsigned char other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    inline Vector2i& operator-=(const unsigned short other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    inline Vector2i& operator-=(const unsigned int other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    inline Vector2i& operator-=(const signed char other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    inline Vector2i& operator-=(const signed short other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    inline Vector2i& operator-=(const signed int other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    inline Vector2i& operator*=(const Vector2i& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    inline Vector2i& operator*=(const float other)
    {
        x *= static_cast<int>(other);
        y *= static_cast<int>(other);
        return *this;
    }

    inline Vector2i& operator*=(const unsigned char other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    inline Vector2i& operator*=(const unsigned short other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    inline Vector2i& operator*=(const unsigned int other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    inline Vector2i& operator*=(const signed char other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    inline Vector2i& operator*=(const signed short other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    inline Vector2i& operator*=(const signed int other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    inline Vector2i& operator/=(const Vector2i& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    inline Vector2i& operator/=(const float other)
    {
        x /= static_cast<int>(other);
        y /= static_cast<int>(other);
        return *this;
    }

    inline Vector2i& operator/=(const unsigned char other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    inline Vector2i& operator/=(const unsigned short other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    inline Vector2i& operator/=(const unsigned int other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    inline Vector2i& operator/=(const signed char other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    inline Vector2i& operator/=(const signed short other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    inline Vector2i& operator/=(const signed int other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    // Comp
    bool operator==(const Vector2i& other)
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(const Vector2i& other)
    {
        return (x != other.x) || (y != other.y);
    }

public:
    int x = 0;
    int y = 0;

public:
    static Vector2i One;
    static Vector2i Zero;
    static Vector2i Up;
    static Vector2i Down;
    static Vector2i Left;
    static Vector2i Right;
};