#include "precompile.h"
#include "Framebuffer.h"

#include "Framework/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Framework
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& aSpecification)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLFramebuffer>(aSpecification);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }
}