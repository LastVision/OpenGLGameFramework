#include "precompile.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Framework
{
    //VERTEX BUFFER

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t aSize)
    {
        GF_PROFILE_FUNCTION();

        glCreateBuffers(1, &myRendererID);
        glBindBuffer(GL_ARRAY_BUFFER, myRendererID);
        glBufferData(GL_ARRAY_BUFFER, aSize, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* aVertices, uint32_t aSize)
    {
        GF_PROFILE_FUNCTION();

        glCreateBuffers(1, &myRendererID);
        glBindBuffer(GL_ARRAY_BUFFER, myRendererID);
        glBufferData(GL_ARRAY_BUFFER, aSize, aVertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        GF_PROFILE_FUNCTION();

        glDeleteBuffers(1, &myRendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        GF_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, myRendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        GF_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void* aData, uint32_t aSize)
    {
        GF_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, myRendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, aSize, aData);
    }

    //INDEX BUFFER

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* aIndices, uint32_t aCount)
        : myCount(aCount)
    {
        GF_PROFILE_FUNCTION();

        glCreateBuffers(1, &myRendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myRendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, aCount * sizeof(uint32_t), aIndices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        GF_PROFILE_FUNCTION();

        glDeleteBuffers(1, &myRendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        GF_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myRendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        GF_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}