#pragma once
#include <NovusTypes.h>

namespace Geometry
{
    struct Triangle
    {
    public:
        vec3 vert1;
        vec3 vert2;
        vec3 vert3;

        const vec3& GetVert(u32 index) const;

        vec3 GetNormal() const;
        f32 GetSteepnessAngle() const;
    };

    struct AABoundingBox
    {
        vec3 min;
        vec3 max;
    };
}