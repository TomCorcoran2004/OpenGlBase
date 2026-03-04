#pragma once
#include <span>
#include <array>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "../Debug/Log.h"

template <typename GenericVertex, typename ...GenericVertexTypes>
class VAO
{
public:
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    VAO()
    {
        glGenVertexArrays(1, &VertexArrayObj);
        glGenBuffers(1, &VertexBufferObj);
        glGenBuffers(1, &ElementBufferObj);

        Bind();

        SetVertexAttributes();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObj);

        UnBind();
    }

    ~VAO()
    {
        glDeleteBuffers(1, &VertexBufferObj);
        glDeleteBuffers(1, &ElementBufferObj);
        glDeleteVertexArrays(1, &VertexArrayObj);
    }

    void Bind()
    {
        glBindVertexArray(VertexArrayObj);
        Bound = true;
    }

    void UnBind()
    {
        glBindVertexArray(0);
        Bound = false;
    }

    void BufferData(const std::span<const GenericVertex>& Vertices, const std::span<const u32>& Indices, u32 DrawType)
    {
        if (Bound == false)
        {
            Base::Log::Error("VAO Not Bound");
                return;
        }
        
        if (Vertices.empty() || Indices.empty())
        {
            IndicesCount = 0;
            Base::Log::Error("Either Vertices or Indices Was Empty");
            return;
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObj);
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(GenericVertex), Vertices.data(), DrawType);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(u32), Indices.data(), DrawType);
    
        IndicesCount = Indices.size();
    }

    void Draw(GLenum PrimitiveType)
    {
        if (Bound == false)
        {
            Base::Log::Error("VAO Not Bound");
                return;
        }
        
        glDrawElements(PrimitiveType, static_cast<GLsizei>(IndicesCount), GL_UNSIGNED_INT, 0);
    }

private:
    template<typename T>
    struct VertexTypeTraits {};

    template<> struct VertexTypeTraits<f32> { static constexpr GLenum Type = GL_FLOAT; static constexpr int NumElements = 1; };
    template<> struct VertexTypeTraits<vec2> { static constexpr GLenum Type = GL_FLOAT; static constexpr int NumElements = 2; };
    template<> struct VertexTypeTraits<vec3> { static constexpr GLenum Type = GL_FLOAT; static constexpr int NumElements = 3; };
    template<> struct VertexTypeTraits<vec4> { static constexpr GLenum Type = GL_FLOAT; static constexpr int NumElements = 4; };
    template<> struct VertexTypeTraits<f64> { static constexpr GLenum Type = GL_DOUBLE; static constexpr int NumElements = 1; };
    template<> struct VertexTypeTraits<dvec2> { static constexpr GLenum Type = GL_DOUBLE; static constexpr int NumElements = 2; };
    template<> struct VertexTypeTraits<dvec3> { static constexpr GLenum Type = GL_DOUBLE; static constexpr int NumElements = 3; };
    template<> struct VertexTypeTraits<dvec4> { static constexpr GLenum Type = GL_DOUBLE; static constexpr int NumElements = 4; };
    template<> struct VertexTypeTraits<u32> { static constexpr GLenum Type = GL_UNSIGNED_INT; static constexpr int NumElements = 1; };
    template<> struct VertexTypeTraits<uvec2> { static constexpr GLenum Type = GL_UNSIGNED_INT; static constexpr int NumElements = 2; };
    template<> struct VertexTypeTraits<uvec3> { static constexpr GLenum Type = GL_UNSIGNED_INT; static constexpr int NumElements = 3; };
    template<> struct VertexTypeTraits<uvec4> { static constexpr GLenum Type = GL_UNSIGNED_INT; static constexpr int NumElements = 4; };
    template<> struct VertexTypeTraits<i32> { static constexpr GLenum Type = GL_INT; static constexpr int NumElements = 1; };
    template<> struct VertexTypeTraits<ivec2> { static constexpr GLenum Type = GL_INT; static constexpr int NumElements = 2; };
    template<> struct VertexTypeTraits<ivec3> { static constexpr GLenum Type = GL_INT; static constexpr int NumElements = 3; };
    template<> struct VertexTypeTraits<ivec4> { static constexpr GLenum Type = GL_INT; static constexpr int NumElements = 4; };
    
    struct VertexAttribute
    {
        GLuint Index;
        GLint NumElements;
        GLenum Type;
        GLboolean Normalized;
        u64 Offset;
    };

    std::array<VertexAttribute, sizeof...(GenericVertexTypes)> VertexAttributes;

    GLuint VertexBufferObj;
    GLuint ElementBufferObj;
    GLuint VertexArrayObj;

    bool Bound = false;

    u64 IndicesCount = 0;

    void SetVertexAttributes()
    {
        if (Bound == false)
        {
            Base::Log::Error("VAO Not Bound");
            return;
        }

        VertexAttributes = GenerateVertexAttributes();

        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObj);

        for (const VertexAttribute& VA : VertexAttributes)
        {
            glVertexAttribPointer(
                VA.Index,
                VA.NumElements,
                VA.Type,
                VA.Normalized,
                sizeof(GenericVertex),
                reinterpret_cast<const void*>(VA.Offset)
            );

            glEnableVertexAttribArray(VA.Index);
        }
    }

    static std::array<VertexAttribute, sizeof...(GenericVertexTypes)> GenerateVertexAttributes()
    {
        std::array<VertexAttribute, sizeof...(GenericVertexTypes)> VertexAttributes;

        u32 Index = 0;
        u64 Offset = 0;

        ((VertexAttributes[Index++] = GenerateVertexAttribute<GenericVertexTypes>(Index, Offset)), ...);

        return VertexAttributes;
    }

    template<typename T>
    static VertexAttribute GenerateVertexAttribute(u32 Index, u64& Offset)
    {
        VertexAttribute VertexAttribute
        {
            .Index = Index,
            .NumElements = VertexTypeTraits<T>::NumElements,
            .Type = VertexTypeTraits<T>::Type,
            .Normalized = false,
            .Offset = Offset,
        };
        
        Index;
        Offset += sizeof(T);

        return VertexAttribute;
    }
};

