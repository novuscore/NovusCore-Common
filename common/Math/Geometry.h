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

        const vec3& GetVert(u32 index) const
        {
            assert(index >= 0 && index < 3);

            if (index == 0)
                return vert1;
            else if (index == 1)
                return vert2;
            else
                return vert3;
        }

        vec3 GetNormal() const
        {
            vec3 a = vert2 - vert1;
            vec3 b = vert3 - vert1;
            return glm::cross(a, b);
        }

        f32 GetSteepnessAngle()
        {
            vec3 normal = glm::normalize(GetNormal());
            f32 flatness = glm::abs(glm::dot(normal, vec3(0, 1, 0)));
            return glm::degrees(acos(flatness));
        }
    };

    struct AABoundingBox
    {
        vec3 min;
        vec3 max;
    };
}