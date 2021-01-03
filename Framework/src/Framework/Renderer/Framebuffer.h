#pragma once

#include "Framework/Core/Core.h"

namespace Framework
{
    struct FramebufferSpecification
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Resize(const uint32_t aWidth, const uint32_t aHeight) = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;

        virtual const FramebufferSpecification& GetSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& aSpecification);
    };
}