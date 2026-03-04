#pragma once
#include <span>
#include <glad/glad.h>
#include "glm/glm.hpp"
namespace Base
{
    namespace Render2D
    {
        bool Init();
        void AddTriangle(const vec2& Point1, const vec2& Point2, const vec2& Point3, const vec4& Color);
        void AddQuad(const vec2& Point1, const vec2& Point2, const vec2& Point3, const vec2& Point4, const vec4& Color);
        void AddLine(const vec2& Start, const vec2& End, f32 Thickness, const vec4& Color);
        void AddArrow(const vec2& Start, const vec2& End, f32 Thickness, const vec4& Color);
        void AddPoint(const vec2& Position, f32 Size, const vec4& Color);
        void Render();
        void Destroy();
    };
}

