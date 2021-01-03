#include "precompile.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Framework
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType aType)
    {
        switch (aType)
        {
        case Framework::ShaderDataType::Float:
        case Framework::ShaderDataType::Float2:
        case Framework::ShaderDataType::Float3:
        case Framework::ShaderDataType::Float4:
        case Framework::ShaderDataType::Mat3:
        case Framework::ShaderDataType::Mat4:
            return GL_FLOAT;
        case Framework::ShaderDataType::Int:
        case Framework::ShaderDataType::Int2:
        case Framework::ShaderDataType::Int3:
        case Framework::ShaderDataType::Int4:
            return GL_INT;
        case Framework::ShaderDataType::Bool:
            return GL_BOOL;
        }

        GF_CORE_ASSERT(false, "Unkown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        GF_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &myRendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        GF_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &myRendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        GF_PROFILE_FUNCTION();

        glBindVertexArray(myRendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        GF_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& aVertexBuffer)
    {
        GF_PROFILE_FUNCTION();

        GF_CORE_ASSERT(aVertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(myRendererID);
        aVertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = aVertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.myType),
                element.myNormalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.myOffset);
            index++;
        }

        myVertexBuffers.emplace_back(aVertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& aIndexBuffer)
    {
        GF_PROFILE_FUNCTION();

        glBindVertexArray(myRendererID);
        aIndexBuffer->Bind();

        myIndexBuffer = aIndexBuffer;
    }
}