#include "Render2D.h"

#include <vector>
#include <array>
#include <glm/glm.hpp>

#include "../Window/Window.h"
#include "../Shader/ShaderManager.h"
#include "../Debug/Log.h"
#include "VAO.h"

namespace PrimitiveShadersSource
{
    const char* Vertex = R"(
    #version 330 core
    layout(location = 0) in vec2 ScreenPos;
    layout(location = 1) in vec4 Color;
    layout(location = 2) in float PointSize;

    out vec4 OurColor;

    uniform vec2 ScreenSize;

    void main() {
        float x = (ScreenPos.x / ScreenSize.x) * 2 - 1;
        float y = 1.0 - (ScreenPos.y / ScreenSize.y) * 2;
        
        gl_Position = vec4(x, y, 0, 1.0);
        gl_PointSize = PointSize;

        OurColor = Color;
    }
    )";

    const char* Fragment = R"(
    #version 330 core
    in vec4 OurColor;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(OurColor);
    }
    )";
}
namespace Base
{
    namespace Render2D
    {
        struct PointVertex
        {
            vec2 Position;
            vec4 Color;
            f32 Size;
        };

        struct PrimitiveVertex
        {
            vec2 Position;
            vec4 Color;
        };

        Base::ShaderProgram* PrimitiveShaderProgram = nullptr;

        VAO<PointVertex, vec2, vec4, f32>* PointsVAO = nullptr;
        VAO<PrimitiveVertex, vec2, vec4>* LinesVAO = nullptr;
        VAO<PrimitiveVertex, vec2, vec4>* TrianglesVAO = nullptr;

        std::vector<PointVertex> PointVertices;
        std::vector<u32> PointIndicies;

        std::vector<PrimitiveVertex> LineVertices;
        std::vector<u32> LineIndices;

        std::vector<PrimitiveVertex> TriangleVertices;
        std::vector<u32> TriangleIndices;

        bool Init()
        {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_PROGRAM_POINT_SIZE);

            Base::ShaderProgramConfig PrimitvesShaderConfig
            {
                .VertexSource = PrimitiveShadersSource::Vertex,
                .FragmentSource = PrimitiveShadersSource::Fragment,
            };
            PrimitiveShaderProgram = new Base::ShaderProgram(PrimitvesShaderConfig);
            assert(PrimitiveShaderProgram);

            PointsVAO = new VAO<PointVertex, vec2, vec4, f32>();
            assert(PrimitiveShaderProgram);

            LinesVAO = new VAO<PrimitiveVertex, vec2, vec4>();
            assert(PrimitiveShaderProgram);

            TrianglesVAO = new VAO<PrimitiveVertex, vec2, vec4>();
            assert(PrimitiveShaderProgram);

            return true;
        }

        void AddTriangle(const vec2& Point1, const vec2& Point2, const vec2& Point3, const vec4& Color)
        {
            PrimitiveVertex Corners[3];
            Corners[0] = PrimitiveVertex(Point1, Color);
            Corners[1] = PrimitiveVertex(Point2, Color);
            Corners[2] = PrimitiveVertex(Point3, Color);

            std::size_t OldVerticesSize = TriangleVertices.size();
            TriangleVertices.resize(OldVerticesSize + 3);
            memcpy(TriangleVertices.data() + OldVerticesSize, Corners, sizeof(Corners));

            u32 Indices[3];
            Indices[0] = OldVerticesSize + 0;
            Indices[1] = OldVerticesSize + 1;
            Indices[2] = OldVerticesSize + 2;

            std::size_t OldIndicesSize = TriangleIndices.size();
            TriangleIndices.resize(OldIndicesSize + 3);
            memcpy(TriangleIndices.data() + OldIndicesSize, Indices, sizeof(Indices));
        }

        void AddQuad(const vec2& Point1, const vec2& Point2, const vec2& Point3, const vec2& Point4, const vec4& Color)
        {
            std::size_t OldVerticesSize = TriangleVertices.size();
            TriangleVertices.resize(OldVerticesSize + 4);
            TriangleVertices[OldVerticesSize + 0] = PrimitiveVertex(Point1, Color);
            TriangleVertices[OldVerticesSize + 1] = PrimitiveVertex(Point2, Color);
            TriangleVertices[OldVerticesSize + 2] = PrimitiveVertex(Point3, Color);
            TriangleVertices[OldVerticesSize + 3] = PrimitiveVertex(Point4, Color);

            std::size_t IndicesSize = TriangleIndices.size();
            TriangleIndices.resize(IndicesSize + 6);
            TriangleIndices[IndicesSize + 0] = OldVerticesSize + 0;
            TriangleIndices[IndicesSize + 1] = OldVerticesSize + 1;
            TriangleIndices[IndicesSize + 2] = OldVerticesSize + 2;
            TriangleIndices[IndicesSize + 3] = OldVerticesSize + 1;
            TriangleIndices[IndicesSize + 4] = OldVerticesSize + 3;
            TriangleIndices[IndicesSize + 5] = OldVerticesSize + 2;
        }

        void AddLine(const vec2& Start, const vec2& End, f32 Thickness, const vec4& Color)
        {
            if (Thickness <= 1.0f)
            {
                PrimitiveVertex Points[2];
                Points[0] = PrimitiveVertex(Start, Color);
                Points[1] = PrimitiveVertex(End, Color);

                std::size_t OldVerticesSize = LineVertices.size();
                LineVertices.resize(OldVerticesSize + 3);
                memcpy(LineVertices.data() + OldVerticesSize, Points, sizeof(Points));

                std::size_t IndicesSize = LineIndices.size();
                LineIndices.resize(IndicesSize + 2);
                LineIndices[IndicesSize + 0] = OldVerticesSize + 0;
                LineIndices[IndicesSize + 1] = OldVerticesSize + 1;
            }
            else
            {
                vec2 Direction = End - Start;
                vec2 NormalisedDirection = glm::normalize(Direction);

                vec2 PerpendicularDirection = { -NormalisedDirection.y, NormalisedDirection.x };

                vec2 Offset = PerpendicularDirection * (Thickness * 0.5f);

                vec2 TopLeft = Start + Offset;
                vec2 BottomLeft = Start - Offset;
                vec2 TopRight = End + Offset;
                vec2 BottomRight = End - Offset;

                AddQuad(TopLeft, TopRight, BottomLeft, BottomRight, Color);
            }
        }

        void AddArrow(const vec2& Start, const vec2& End, f32 Thickness, const vec4& Color)
        {
            vec2 Direction = End - Start;
            vec2 NormalisedDirection = glm::normalize(Direction);

            vec2 PerpendicularDirection = { -NormalisedDirection.y, NormalisedDirection.x };

            vec2 Offset = PerpendicularDirection * (Thickness * 0.5f);

            vec2 ArrowBase = End - NormalisedDirection * Thickness * 6.f;
            vec2 Point2 = ArrowBase - PerpendicularDirection * Thickness * 3.f;
            vec2 Point3 = ArrowBase + PerpendicularDirection * Thickness * 3.f;
            AddTriangle(End, Point2, Point3, Color);

            vec2 TopLeft = Start + Offset;
            vec2 BottomLeft = Start - Offset;
            vec2 TopRight = ArrowBase + Offset;
            vec2 BottomRight = ArrowBase - Offset;

            AddQuad(TopLeft, BottomLeft, TopRight, BottomRight, Color);
        }

        void AddPoint(const vec2& Position, f32 Size, const vec4& Color)
        {
            PointVertex Point = PointVertex(Position, Color, Size);
            std::size_t OldVerticesSize = PointVertices.size();
            PointVertices.resize(OldVerticesSize + 1);
            memcpy(PointVertices.data() + OldVerticesSize, &Point, sizeof(Point));

            std::size_t IndicesSize = PointIndicies.size();
            PointIndicies.resize(IndicesSize + 1);
            PointIndicies[IndicesSize] = OldVerticesSize;
        }

        void Render()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (PrimitiveShaderProgram == nullptr)
            {
                Base::Log::Error("PrimitveShaderProgram == nullptr");
                return;
            }

            if (PointsVAO == nullptr)
            {
                Base::Log::Error("PointsVAO == nullptr");
                return;
            }

            if (LinesVAO == nullptr)
            {
                Base::Log::Error("LinesVAO == nullptr");
                return;
            }

            if (TrianglesVAO == nullptr)
            {
                Base::Log::Error("TrianglesVAO == nullptr");
                return;
            }

            PrimitiveShaderProgram->Use();

            vec2 WindowSize = Base::Window::GetWindowSize();
            PrimitiveShaderProgram->SetUniform("ScreenSize", WindowSize);

            if (PointVertices.size())
            {
                PointsVAO->Bind();
                PointsVAO->BufferData(PointVertices, PointIndicies, GL_STREAM_DRAW);
                PointsVAO->Draw();
                PointsVAO->UnBind();
            }
            PointVertices.clear();
            LineIndices.clear();

            if (LineVertices.size())
            {
                LinesVAO->Bind();
                LinesVAO->BufferData(LineVertices, LineIndices, GL_STREAM_DRAW);
                LinesVAO->Draw();
                LinesVAO->UnBind();
            }
            LineVertices.clear();
            LineIndices.clear();

            if (TriangleVertices.size())
            {
                TrianglesVAO->Bind();
                TrianglesVAO->BufferData(TriangleVertices, TriangleIndices, GL_STREAM_DRAW);
                TrianglesVAO->Draw();
                TrianglesVAO->UnBind();
            }
            TriangleVertices.clear();
            TriangleIndices.clear();
        }

        void Destroy()
        {
            delete PrimitiveShaderProgram;
            delete PointsVAO;
            delete LinesVAO;
            delete TrianglesVAO;
        }

    }
}