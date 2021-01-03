#include "precompile.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Framework
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexArray>();
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }
}