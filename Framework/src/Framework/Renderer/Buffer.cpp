#include "precompile.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Framework
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t aSize)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(aSize);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* aVertices, uint32_t aSize)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(aVertices, aSize);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* aIndices, uint32_t aCount)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLIndexBuffer>(aIndices, aCount);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }
}