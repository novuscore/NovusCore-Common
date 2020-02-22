#include "Vector2i.h"
#include "Vector2.h"
#include "Vector3.h"

// Right handed coordinate system, positive X points right, positive Y points up
Vector2i Vector2i::One(1, 1);
Vector2i Vector2i::Zero(0, 0);
Vector2i Vector2i::Up(0, 1);
Vector2i Vector2i::Down(0, -1);
Vector2i Vector2i::Left(-1, 0);
Vector2i Vector2i::Right(1, 0);

Vector2i::Vector2i(const Vector2& v2)
{
    x = static_cast<int>(v2.x);
    y = static_cast<int>(v2.y);
}

Vector2i::Vector2i(const Vector3& v3)
{
    x = static_cast<int>(v3.x);
    y = static_cast<int>(v3.y);
}