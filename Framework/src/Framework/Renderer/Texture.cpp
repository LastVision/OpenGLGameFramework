#include "precompile.h"
#include "Texture.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Framework
{
    Ref<Texture2D> Texture2D::Create(const uint32_t aWidth, const uint32_t aHeight)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTexture2D>(aWidth, aHeight);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& aPath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTexture2D>(aPath);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }
}